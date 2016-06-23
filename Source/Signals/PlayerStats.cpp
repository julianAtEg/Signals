// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "PlayerStats.h"
#include "AttackClass.h"
#include "Random.h"

UPlayerStats::UPlayerStats(FObjectInitializer const & init)
: Super(init)
, HitPoints( 0 )
, MaxHitPoints( 0 )
, Speed( 0 )
, Strength( 0 )
, Dexterity( 0 )
, Defence()
, Level( 1 )
{
	Defence.SetNum(EAttackClass::NumAttackClasses, true);
}

void UPlayerStats::Load(FString const & filePath)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading player stats from %s"),*filePath);

	FXmlFile dataXml;
	if (!dataXml.LoadFile(filePath, EConstructMethod::Type::ConstructFromFile))
	{
		UE_LOG(SignalsLog, Error, TEXT("Error loading %s"), *filePath);
		return;
	}

	auto rootNode = dataXml.GetRootNode();
	fromXml(rootNode);
}

TArray<FString> UPlayerStats::GetActions() const
{
	return TArray<FString>();
}

int UPlayerStats::ComputeAttack(Random * rng, int base, int levelScale, FString const & action) const
{
	auto multiplier = ((1.0f + Level / 100.0f)*Level);
	float luckScalar = 0.85f + rng->Next01();
	auto attack = (int)(luckScalar*(float(base) + levelScale*multiplier));
	return attack;
}

int UPlayerStats::ComputeDefence(Random * rng, int levelScale, EAttackClass attackClass) const
{
	float luckScalar = 0.80f + rng->Next01();
	int defence = (int)(luckScalar*levelScale*(1.0f + Level / 100.0f)*Defence[attackClass]);
	return defence;
}

int UPlayerStats::ComputeRegain(Random * rng, int base, int levelScale, FString const & action) const
{
	float luckScalar = 0.95f + rng->Next01();
	auto regain = (int)(luckScalar*(float(base) + levelScale*Level));
	return regain;
}

void UPlayerStats::fromXml(FXmlNode * const root)
{
	auto hpStr = root->GetAttribute(TEXT("hp"));
	HitPoints = FCString::Atoi(*hpStr);

	auto maxHpStr = root->GetAttribute(TEXT("maxHP"));
	MaxHitPoints = FCString::Atoi(*maxHpStr);
	
	auto speedStr = root->GetAttribute(TEXT("speed"));
	Speed = FCString::Atoi(*speedStr);

	auto strengthStr = root->GetAttribute(TEXT("strength"));
	Strength = FCString::Atoi(*strengthStr);

	auto levelStr = root->GetAttribute(TEXT("level"));
	Level = FCString::Atoi(*levelStr);

	auto dexterityStr = root->GetAttribute(TEXT("dexterity"));
	Dexterity = FCString::Atoi(*dexterityStr);

	auto evasionStr = root->GetAttribute(TEXT("evasion"));
	Evasion = FCString::Atoi(*evasionStr);

	auto defNode = root->FindChildNode(TEXT("defence"));
	auto attrs = defNode->GetAttributes();
	for (auto & attr : attrs)
	{
		auto traitStr = attr.GetTag();
		auto value = FCString::Atoi(*attr.GetValue());
		auto index = (int)AttackClass::FromString(traitStr);
		Defence[index] = value;
	}
}