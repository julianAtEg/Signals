#pragma once

enum EStatClass
{
	Undefined,
	HitPoints,
	Speed,
	Dexterity,
	Evasion,
	Strength,
	PhysicalDefence,
	FireDefence,
	IceDefence,
	ElectricalDefence,
	LightDefence,
	PlasmaDefence,
	SoundDefence,
	PoisonDefence,
	BacterialDefence,
	ViralDefence,
	Energy,
	MaxHitPoints,
};

namespace StatType
{
	EStatClass FromString(FString const & text);
	FString ToString(EStatClass state);
	FString ToAbbreviatedString(EStatClass stat);
	bool AffectsSchedule(EStatClass stat);
}