// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "PlayerStats.h"
#include "AttackClass.h"
#include "Random.h"

UPlayerStats::UPlayerStats(FObjectInitializer const & init)
	: Super(init)
	, HitPoints(0)
	, MaxHitPoints(0)
	, Speed(0)
	, Strength(0)
	, Dexterity(0)
	, Defence()
	, Level(1)
	, IconIndex(0)
{
	Defence.SetNum(EAttackClass::NumAttackClasses, true);
}

void UPlayerStats::Load(FString const & filePath)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading player stats from %s"), *filePath);

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

float UPlayerStats::ComputeShieldFactor(Random *) const
{
	return(0.5f);
}

void UPlayerStats::fromXml(FXmlNode * const root)
{
	auto iconStr = root->GetAttribute(TEXT("icon"));
	IconIndex = FCString::Atoi(*iconStr);

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

int UPlayerStats::getStat(EStatClass stat) const
{
	switch (stat)
	{
	case EStatClass::HitPoints:
		return HitPoints;

	case EStatClass::PhysicalDefence:
		return Defence[EAttackClass::Physical];

	case EStatClass::FireDefence:
		return Defence[EAttackClass::Fire];

	case EStatClass::IceDefence:
		return Defence[EAttackClass::Ice];

	case EStatClass::ElectricalDefence:
		return Defence[EAttackClass::Electrical];

	case EStatClass::LightDefence:
		return Defence[EAttackClass::Light];

	case EStatClass::PlasmaDefence:
		return Defence[EAttackClass::Plasma];

	case EStatClass::SoundDefence:
		return Defence[EAttackClass::Sound];

	case EStatClass::PoisonDefence:
		return Defence[EAttackClass::Poison];

	case EStatClass::BacterialDefence:
		return Defence[EAttackClass::Bacterial];

	case EStatClass::ViralDefence:
		return Defence[EAttackClass::Viral];

	case EStatClass::Dexterity:
		return Dexterity;

	case EStatClass::Evasion:
		return Evasion;

	case EStatClass::Speed:
		return Speed;

	case EStatClass::Strength:
		return Strength;

	case EStatClass::Energy:
		return getEnergy();

	case EStatClass::MaxHitPoints:
		return MaxHitPoints;

	case EStatClass::Undefined:
	default:
		UE_LOG(SignalsLog, Error, TEXT("Undefined stat type"));
		return 0;
	}
}

void UPlayerStats::setStat(EStatClass stat, int value)
{
	switch (stat)
	{
	case EStatClass::HitPoints:
		HitPoints = FMath::Min(value, MaxHitPoints);
		break;

	case EStatClass::PhysicalDefence:
		Defence[EAttackClass::Physical] = value;
		break;

	case EStatClass::FireDefence:
		Defence[EAttackClass::Fire] = value;
		break;

	case EStatClass::IceDefence:
		Defence[EAttackClass::Ice] = value;
		break;

	case EStatClass::ElectricalDefence:
		Defence[EAttackClass::Electrical] = value;
		break;

	case EStatClass::LightDefence:
		Defence[EAttackClass::Light] = value;
		break;

	case EStatClass::PlasmaDefence:
		Defence[EAttackClass::Plasma] = value;
		break;

	case EStatClass::SoundDefence:
		Defence[EAttackClass::Sound] = value;
		break;

	case EStatClass::PoisonDefence:
		Defence[EAttackClass::Poison] = value;
		break;

	case EStatClass::BacterialDefence:
		Defence[EAttackClass::Bacterial] = value;
		break;

	case EStatClass::ViralDefence:
		Defence[EAttackClass::Viral] = value;
		break;

	case EStatClass::Dexterity:
		Dexterity = value;
		break;

	case EStatClass::Evasion:
		Evasion = value;
		break;

	case EStatClass::Speed:
		Speed = value;
		break;

	case EStatClass::Strength:
		Strength = value;
		break;

	case EStatClass::Energy:
		setEnergy(value);
		break;

	case EStatClass::MaxHitPoints:
		MaxHitPoints = value;
		HitPoints = FMath::Min(HitPoints, MaxHitPoints);
		break;

	case EStatClass::Undefined:
		UE_LOG(SignalsLog, Error, TEXT("Undefined stat type"));
		break;
	}
}

int UPlayerStats::getEnergy() const
{
	return 0;
}

void UPlayerStats::setEnergy(int)
{
}

void UPlayerStats::ApplyStatChange(EStatClass stat, int delta, bool transient)
{
	if (stat == EStatClass::HitPoints)
	{
		HitPoints = FMath::Clamp(HitPoints + delta, 0, MaxHitPoints);
	}
	else
	{
		auto value = getStat(stat);
		value = FMath::Clamp(value + delta, 0, 100);
		setStat(stat, value);
	}
}

void UPlayerStats::BeginBattle()
{
}

void UPlayerStats::EndBattle()
{
}