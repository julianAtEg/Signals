#include "Signals.h"
#include "StatType.h"

static EStatClass defenceFromString(FString const & defStr);

namespace StatType
{
	EStatClass FromString(FString const & typeStr)
	{
		auto lcTypeStr = typeStr.ToLower();
		if (lcTypeStr == TEXT("hitpoints"))
		{
			return EStatClass::HitPoints;
		}
		else if (lcTypeStr == TEXT("maxhitpoints"))
		{
			return EStatClass::MaxHitPoints;
		}
		else if (lcTypeStr == TEXT("speed"))
		{
			return EStatClass::Speed;
		}
		else if (lcTypeStr == TEXT("energy"))
		{
			return EStatClass::Energy;
		}
		else if (lcTypeStr == TEXT("strength"))
		{
			return EStatClass::Strength;
		}
		else if (lcTypeStr.StartsWith("defence"))
		{
			return defenceFromString(lcTypeStr);
		}
		else if (lcTypeStr == TEXT("dexterity"))
		{
			return EStatClass::Dexterity;
		}
		else if (lcTypeStr == TEXT("evasion"))
		{
			return EStatClass::Evasion;
		}
		else
		{
			UE_LOG(SignalsLog,Error, TEXT("Undefined stat type '%s'"), *typeStr);
			return EStatClass::Undefined;
		}
	}

	FString ToString(EStatClass stat)
	{
		switch (stat)
		{
			case EStatClass::HitPoints:
				return "hitpoints";

			case EStatClass::MaxHitPoints:
				return "maxhitpoints";

			case EStatClass::Speed:
				return "speed";

			case EStatClass::Energy:
				return "energy";

			case EStatClass::Strength:
				return "strength";

			case EStatClass::FireDefence:
				return "defence.fire";

			case EStatClass::IceDefence:
				return "defence.ice";

			case EStatClass::ElectricalDefence:
				return "defence.electrical";

			case EStatClass::LightDefence:
				return "defence.light";

			case EStatClass::PlasmaDefence:
				return "defence.plasma";

			case EStatClass::SoundDefence:
				return "defence.sound";

			case EStatClass::PoisonDefence:
				return "defence.poison";

			case EStatClass::BacterialDefence:
				return "defence.bacterial";

			case EStatClass::ViralDefence:
				return "defence.viral";

			case EStatClass::Dexterity:
				return "dexterity";

			case EStatClass::Evasion:
				return "evasion";

			default:
				return "undefined";
		}
	}

	FString ToAbbreviatedString(EStatClass stat)
	{
		switch (stat)
		{
		case EStatClass::HitPoints:
			return "HP";

		case EStatClass::MaxHitPoints:
			return "MAXHP";

		case EStatClass::Speed:
			return "SPD";

		case EStatClass::Energy:
			return "ERG";

		case EStatClass::Strength:
			return "STR";

		case EStatClass::FireDefence:
			return "FIR-D";

		case EStatClass::IceDefence:
			return "ICE-D";

		case EStatClass::ElectricalDefence:
			return "ELE-D";

		case EStatClass::LightDefence:
			return "LGT-D";

		case EStatClass::PlasmaDefence:
			return "PLA-D";

		case EStatClass::SoundDefence:
			return "SND-D";

		case EStatClass::PoisonDefence:
			return "POI-D";

		case EStatClass::BacterialDefence:
			return "BAC-D";

		case EStatClass::ViralDefence:
			return "VIR-D";

		case EStatClass::Dexterity:
			return "DEX";

		case EStatClass::Evasion:
			return "EVA";

		default:
			return "???";
		}
	}

	bool AffectsSchedule(EStatClass stat)
	{
		switch (stat)
		{
			case EStatClass::Speed:
				return true;

			default:
				return false;
		}
	}
}

static EStatClass defenceFromString(FString const & defStr)
{
	int dotPos;
	if (!defStr.FindChar('.', dotPos))
	{
		return EStatClass::Undefined;
	}

	auto typeStr = defStr.RightChop(dotPos + 1);
	if (typeStr == TEXT("physical"))
	{
		return EStatClass::PhysicalDefence;
	}
	else if (typeStr == TEXT("fire"))
	{
		return EStatClass::FireDefence;
	}
	else if (typeStr == TEXT("ice"))
	{
		return EStatClass::IceDefence;
	}
	else if (typeStr == TEXT("electrical"))
	{
		return EStatClass::ElectricalDefence;
	}
	else if (typeStr == TEXT("light"))
	{
		return EStatClass::LightDefence;
	}
	else if (typeStr == TEXT("plasma"))
	{
		return EStatClass::PlasmaDefence;
	}
	else if (typeStr == TEXT("sound"))
	{
		return EStatClass::SoundDefence;
	}
	else if (typeStr == TEXT("poison"))
	{
		return EStatClass::PoisonDefence;
	}
	else if (typeStr == TEXT("bacterial"))
	{
		return EStatClass::BacterialDefence;
	}
	else if (typeStr == TEXT("viral"))
	{
		return EStatClass::ViralDefence;
	}
	else
	{
		return EStatClass::Undefined;
	}
}
