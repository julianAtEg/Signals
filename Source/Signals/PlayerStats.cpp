// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "PlayerStats.h"
#include "AttackClass.h"
#include "Random.h"

UPlayerStats::UPlayerStats(FObjectInitializer const & init)
: Super(init)
, Level(1)
, IconIndex(0)
, Status(0x00000003)
, _hitPoints()
, _maxHitPoints()
, _speed()
, _strength()
, _dexterity()
, _evasion()
, _defence()
{

	_defence.SetNum(EAttackClass::NumAttackClasses, true);
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
	auto defValue = _defence[attackClass].GetValue();
	int defence = (int)(luckScalar*levelScale*(1.0f + Level / 100.0f)*defValue);
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
	_hitPoints.SetBaseValue( FCString::Atoi(*hpStr) );

	auto maxHpStr = root->GetAttribute(TEXT("maxHP"));
	_maxHitPoints.SetBaseValue(FCString::Atoi(*maxHpStr));

	auto speedStr = root->GetAttribute(TEXT("speed"));
	_speed.SetBaseValue( FCString::Atoi(*speedStr) );

	auto strengthStr = root->GetAttribute(TEXT("strength"));
	_strength.SetBaseValue(FCString::Atoi(*strengthStr));

	auto levelStr = root->GetAttribute(TEXT("level"));
	Level = FCString::Atoi(*levelStr);

	auto dexterityStr = root->GetAttribute(TEXT("dexterity"));
	_dexterity.SetBaseValue( FCString::Atoi(*dexterityStr) );

	auto evasionStr = root->GetAttribute(TEXT("evasion"));
	_evasion.SetBaseValue(FCString::Atoi(*evasionStr));

	auto defNode = root->FindChildNode(TEXT("defence"));
	auto attrs = defNode->GetAttributes();
	for (auto & attr : attrs)
	{
		auto traitStr = attr.GetTag();
		auto value = FCString::Atoi(*attr.GetValue());
		auto index = (int)AttackClass::FromString(traitStr);
		_defence[index].SetBaseValue(value);
	}
}

int UPlayerStats::GetStat(EStatClass stat) const
{
	int value = 0;
	switch (stat)
	{
	case EStatClass::HitPoints:
		value = _hitPoints.GetValue();
		break;

	case EStatClass::PhysicalDefence:
		value = _defence[EAttackClass::Physical].GetValue();
		break;

	case EStatClass::FireDefence:
		value = _defence[EAttackClass::Fire].GetValue();
		break;

	case EStatClass::IceDefence:
		value = _defence[EAttackClass::Ice].GetValue();
		break;

	case EStatClass::ElectricalDefence:
		value = _defence[EAttackClass::Electrical].GetValue();
		break;

	case EStatClass::LightDefence:
		value = _defence[EAttackClass::Light].GetValue();
		break;

	case EStatClass::PlasmaDefence:
		value = _defence[EAttackClass::Plasma].GetValue();
		break;

	case EStatClass::SoundDefence:
		value = _defence[EAttackClass::Sound].GetValue();
		break;

	case EStatClass::PoisonDefence:
		value = _defence[EAttackClass::Poison].GetValue();
		break;

	case EStatClass::BacterialDefence:
		value = _defence[EAttackClass::Bacterial].GetValue();
		break;

	case EStatClass::ViralDefence:
		value = _defence[EAttackClass::Viral].GetValue();
		break;

	case EStatClass::Dexterity:
		value = _dexterity.GetValue();
		break;

	case EStatClass::Evasion:
		value = _evasion.GetValue();
		break;

	case EStatClass::Speed:
		value = _speed.GetValue();
		break;

	case EStatClass::Strength:
		value = _strength.GetValue();
		break;

	case EStatClass::Energy:
		value = getEnergy();
		break;

	case EStatClass::MaxHitPoints:
		value = _maxHitPoints.GetValue();
		break;

	case EStatClass::Undefined:
	default:
		UE_LOG(SignalsLog, Error, TEXT("Undefined stat type"));
		break;
	}

	return value;
}

void UPlayerStats::SetStat(EStatClass stat, int value)
{
	switch (stat)
	{
	case EStatClass::HitPoints:
		_hitPoints.SetBaseValue(value);
		break;

	case EStatClass::PhysicalDefence:
		_defence[EAttackClass::Physical] = value;
		break;

	case EStatClass::FireDefence:
		_defence[EAttackClass::Fire] = value;
		break;

	case EStatClass::IceDefence:
		_defence[EAttackClass::Ice] = value;
		break;

	case EStatClass::ElectricalDefence:
		_defence[EAttackClass::Electrical] = value;
		break;

	case EStatClass::LightDefence:
		_defence[EAttackClass::Light] = value;
		break;

	case EStatClass::PlasmaDefence:
		_defence[EAttackClass::Plasma] = value;
		break;

	case EStatClass::SoundDefence:
		_defence[EAttackClass::Sound] = value;
		break;

	case EStatClass::PoisonDefence:
		_defence[EAttackClass::Poison] = value;
		break;

	case EStatClass::BacterialDefence:
		_defence[EAttackClass::Bacterial] = value;
		break;

	case EStatClass::ViralDefence:
		_defence[EAttackClass::Viral] = value;
		break;

	case EStatClass::Dexterity:
		_dexterity = value;
		break;

	case EStatClass::Evasion:
		_evasion = value;
		break;

	case EStatClass::Speed:
		_speed = value;
		break;

	case EStatClass::Strength:
		_strength = value;
		break;

	case EStatClass::Energy:
		setEnergy(value);
		break;

	case EStatClass::MaxHitPoints:
		_maxHitPoints.SetBaseValue(value);
		_hitPoints.SetMax(_maxHitPoints.GetValue());
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

void UPlayerStats::BeginBattle()
{
}

void UPlayerStats::EndBattle()
{
	_hitPoints.ClearModifiers();
	_maxHitPoints.ClearModifiers();
	_hitPoints.SetMax(_maxHitPoints.GetBaseValue());
	_speed.ClearModifiers();
	_strength.ClearModifiers();
	_dexterity.ClearModifiers();
	_evasion.ClearModifiers();
	for (auto & stat : _defence)
	{
		stat.ClearModifiers();
	}
}

void UPlayerStats::ApplyStatChange(EStatClass stat, int delta)
{
	auto theStat = GetStatRef(stat);
	if (theStat != nullptr)
	{
		theStat->SetBaseValue(theStat->GetBaseValue() + delta);
	}
}

void UPlayerStats::AddStatModifier(StatModifier * modifier)
{
	auto stat = GetStatRef(modifier->Stat);
	stat->AddModifier(modifier);
}

void UPlayerStats::RemoveStatModifier(StatModifier * modifier)
{
	auto stat = GetStatRef(modifier->Stat);
	stat->RemoveModifier(modifier);
}

Stat * UPlayerStats::GetStatRef(EStatClass stat)
{
	switch (stat)
	{
	case EStatClass::HitPoints:
		return &_hitPoints;

	case EStatClass::PhysicalDefence:
		return &_defence[EAttackClass::Physical];

	case EStatClass::FireDefence:
		return &_defence[EAttackClass::Fire];

	case EStatClass::IceDefence:
		return &_defence[EAttackClass::Ice];

	case EStatClass::ElectricalDefence:
		return &_defence[EAttackClass::Electrical];

	case EStatClass::LightDefence:
		return &_defence[EAttackClass::Light];

	case EStatClass::PlasmaDefence:
		return &_defence[EAttackClass::Plasma];

	case EStatClass::SoundDefence:
		return &_defence[EAttackClass::Sound];

	case EStatClass::PoisonDefence:
		return &_defence[EAttackClass::Poison];

	case EStatClass::BacterialDefence:
		return &_defence[EAttackClass::Bacterial];

	case EStatClass::ViralDefence:
		return &_defence[EAttackClass::Viral];

	case EStatClass::Dexterity:
		return &_dexterity;

	case EStatClass::Evasion:
		return &_evasion;

	case EStatClass::Speed:
		return &_speed;

	case EStatClass::Strength:
		return &_strength;

	case EStatClass::Energy:
		return nullptr;

	case EStatClass::MaxHitPoints:
		return &_maxHitPoints;
		break;

	case EStatClass::Undefined:
	default:
		UE_LOG(SignalsLog, Error, TEXT("Undefined stat type"));
		return nullptr;
	}
}