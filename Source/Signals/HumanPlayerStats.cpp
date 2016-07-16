// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "HumanPlayerStats.h"
#include "Random.h"
#include "Combat.h"
#include "Action.h"
#include "Item.h"
#include "SignalsInstance.h"
#include "ItemActionNode.h"

UHumanPlayerStats::UHumanPlayerStats(FObjectInitializer const & init)
: Super(init)
, EXP(0)
, _abilities()
, _abilityLevelMap()
, _levelCurve()
, _hpCurve()
, _nextExpLevel(0)
, _skills()
{
	// TODO: remove this hack.
	_skills.Add(BattleSkill::BoostSkill);
}

int UHumanPlayerStats::ComputeAttack( Random * rng, int base, int levelScale, FString const & action ) const
{
	auto ability = FindAbility(action);
	float luckScalar = Combat::ComputeLuck(rng, ability->Skill);
	auto multiplier = ((1.0f + Level / 100.0f)*ability->Skill);
	auto attack = (int)(luckScalar*(float(base) + levelScale*multiplier));
	return attack;
}

int UHumanPlayerStats::ComputeRegain(Random * rng, int base, int levelScale, FString const & action) const
{
	auto ability = FindAbility(action);
	float luckScalar = Combat::ComputeLuck(rng, ability->Skill);
	auto multiplier = ((1.0f + Level / 100.0f)*ability->Skill);
	auto regain = (int)(luckScalar*(float(base) + levelScale*multiplier));
	return regain;
}

void UHumanPlayerStats::SetInitialValues()
{
	HitPoints = MaxHitPoints = _hpCurve.GetValue(1);
	// TODO: speed, strength...

	EXP = 0;
	_nextExpLevel = _levelCurve.GetValue(2);
}

void UHumanPlayerStats::AddSkill(BattleSkill skill)
{
	_skills.Add(skill);
}

bool UHumanPlayerStats::HasSkill(BattleSkill skill)
{
	return _skills.Contains(skill);
}

int UHumanPlayerStats::GetExpToNextLevel() const
{
	return(_nextExpLevel - EXP);
}

void UHumanPlayerStats::NextLevel(TArray<FString> & newAbilities)
{
	UE_LOG(SignalsLog, Log, TEXT("Level up!"));
	++Level;
	newAbilities.Empty();
	auto result = _abilityLevelMap.Find(Level);
	if (result != nullptr)
	{
		for (auto & ab : *result)
		{
			newAbilities.Add(ab.Name);
			AddAbility(ab);
		}
	}
}

TArray<FString> UHumanPlayerStats::GetAvailableActionNames() const
{
	TArray<FString> result;
	for (auto & ab : _abilities)
	{
		result.Add(ab.Name);
	}
	return result;
}

void UHumanPlayerStats::fromXml(FXmlNode * const root)
{
	Super::fromXml(root);

	auto expStr = root->GetAttribute(TEXT("exp"));
	EXP = FCString::Atoi(*expStr);

	_abilities.Empty();
	auto absNode = root->FindChildNode(TEXT("abilities"));
	auto & children = absNode->GetChildrenNodes();
	for (auto & abNode : children)
	{
		Ability ability;
		ability.FromXml(abNode, true);
		_abilities.Add(ability);
	}

	_inventory.Clear();
	auto invNode = root->FindChildNode(TEXT("inventory"));
	_inventory.FromXml(invNode);

	_equippedItems.Empty();
	auto itemsNode = root->FindChildNode(TEXT("equippedItems"));
	auto itemsStr = itemsNode->GetContent();
	TArray<FString> strItems;
	itemsStr.ParseIntoArray(strItems, TEXT(","));
	for (auto & item : strItems)
	{
		_equippedItems.Add(FCString::Atoi(*item));
	}
}

void UHumanPlayerStats::AddAbility(Ability const & ability)
{
	auto existing = FindAbility(ability.Name);
	if (existing != nullptr)
	{
		UE_LOG(SignalsLog, Warning, TEXT("Duplicated ability '%s'"), *ability.Name);
		return;
	}

	_abilities.Add(ability);
}

Ability const * UHumanPlayerStats::FindAbility(FString const & name) const
{
	for (int i = 0; i < _abilities.Num(); ++i)
	{
		auto ability = &_abilities[i];
		if (ability->Name == name)
		{
			return ability;
		}
	}

	return nullptr;
}

void UHumanPlayerStats::LoadStaticData(FString const & playerName)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading static data for %s"), *playerName);

	auto fileName = playerName + TEXT("-data.xml");
	auto contentDir = FPaths::GameContentDir();
	auto filePath = FPaths::Combine(*contentDir, TEXT("Data"), *fileName);
	FXmlFile dataXml;
	if (!dataXml.LoadFile(filePath, EConstructMethod::Type::ConstructFromFile))
	{
		UE_LOG(SignalsLog, Error, TEXT("Error loading %s"),*fileName);
		return;
	}

	auto root = dataXml.GetRootNode();
	auto skillsNode = root->FindChildNode(TEXT("skilltree"));
	auto & kids = skillsNode->GetChildrenNodes();
	for (auto & levelNode : kids)
	{
		TArray<Ability> levelAbilities;
		auto levelStr = levelNode->GetAttribute(TEXT("value"));
		int level = FCString::Atoi(*levelStr);
		auto abNodes = levelNode->GetChildrenNodes();
		for (auto & abNode : abNodes)
		{
			Ability ability;
			ability.FromXml(abNode, false);
			
			levelAbilities.Emplace(ability);
		}
		_abilityLevelMap.Emplace(level, levelAbilities);
	}

	// Load curve that defines MaxHP as a function of Level.
	auto hpNode = root->FindChildNode(TEXT("hpCurve"));
	_hpCurve.FromXml(hpNode);

	// Load curve that defines EXP values for levelling up.
	auto levelNode = root->FindChildNode(TEXT("levelCurve"));
	_levelCurve.FromXml(levelNode);
}

TArray<FString> UHumanPlayerStats::GetActions() const
{
	TArray<FString> result;
	for (auto & ab : _abilities)
	{
		result.Add(ab.Name);
	}
	return result;
}

void UHumanPlayerStats::EquipItem(int itemID)
{
	UE_LOG(SignalsLog, Log, TEXT("Player equipping item %d"), itemID);

	_inventory.RemoveItem(itemID);
	auto item = Item::FindByID(itemID);
	auto equipNode = item->GetEquipNode();
	for (int i = 0; i < equipNode->GetChildCount(); ++i)
	{
		auto child = equipNode->GetChild(i);
		auto ian = (ItemActionNode *)child;
		ian->Apply(this);
	}

	_equippedItems.Add(itemID);
}

void UHumanPlayerStats::UnequipItem(int itemID)
{
	UE_LOG(SignalsLog, Log, TEXT("Player unequipping item %d"), itemID);

	_equippedItems.Remove(itemID);
	auto item = Item::FindByID(itemID);
	auto equipNode = item->GetEquipNode();
	for (int i = 0; i < equipNode->GetChildCount(); ++i)
	{
		auto child = equipNode->GetChild(i);
		auto ian = (ItemActionNode *)child;
		ian->Remove(this);
	}
	_inventory.AddItem(itemID);
}

void UHumanPlayerStats::ApplyStatChange(EStatClass stat, int delta, bool transient)
{
	if (transient)
	{
		TransientStatChange delta1;
		delta1.Stat = stat;
		delta1.Value = GetStat(stat);
		_transientStatChanges.Emplace(delta1);

		// Changing a max means caching the max'd stat, too.
		if (stat == EStatClass::MaxHitPoints)
		{
			TransientStatChange delta2;
			delta2.Stat = EStatClass::HitPoints;
			delta2.Value = HitPoints;
			_transientStatChanges.Emplace(delta2);
		}
	}

	UPlayerStats::ApplyStatChange(stat, delta, transient);
}

int UHumanPlayerStats::getEnergy() const
{
	auto world = GEngine->GetWorld();
	auto instance = Cast<USignalsInstance>(UGameplayStatics::GetGameInstance(world));
	return instance->Ergs;
}

void UHumanPlayerStats::setEnergy(int value)
{
	auto world = GEngine->GetWorld();
	auto instance = Cast<USignalsInstance>(UGameplayStatics::GetGameInstance(world));
	instance->Ergs = value;
}

void UHumanPlayerStats::BeginBattle()
{
	_transientStatChanges.Empty();
}

void UHumanPlayerStats::EndBattle()
{
	// Apply in reverse order to maintain consistency.
	for (int i = _transientStatChanges.Num() - 1; i >= 0; --i)
	{
		SetStat(_transientStatChanges[i].Stat, _transientStatChanges[i].Value);
	}
	_transientStatChanges.Empty();
}
