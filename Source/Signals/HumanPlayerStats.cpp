// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "HumanPlayerStats.h"
#include "XmlParser.h"

UHumanPlayerStats::UHumanPlayerStats(FObjectInitializer const & init)
: Super(init)
, Level(0)
, _abilities()
, _abilityLevelMap()
, _levelCurve()
, _hpCurve()
{

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
			_abilities.Add(ab);
		}
	}
}

void UHumanPlayerStats::fromXml(FXmlNode * const root)
{
	Super::fromXml(root);

	auto levelStr = root->GetAttribute(TEXT("level"));
	Level = FCString::Atoi(*levelStr);

	auto expStr = root->GetAttribute(TEXT("exp"));
	EXP = FCString::Atoi(*expStr);

	auto absNode = root->FindChildNode(TEXT("abilities"));
	auto & children = absNode->GetChildrenNodes();
	for (auto & abNode : children)
	{
		Ability ability;
		ability.FromXml(abNode, true);
		_abilities.Add(ability);
	}
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
	auto skillsNode = root->FindChildNode(TEXT("skilltree"))
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

