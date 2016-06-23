#include "Signals.h"
#include "StatType.h"

namespace StatType
{
	EStatClass FromString(FString const & typeStr)
	{
		auto lcTypeStr = typeStr.ToLower();
		if (lcTypeStr == TEXT("hitpoints"))
		{
			return EStatClass::HitPoints;
		}
		else if (lcTypeStr == TEXT("morale"))
		{
			return EStatClass::Morale;
		}
		else if (lcTypeStr == TEXT("energy"))
		{
			return EStatClass::Energy;
		}
		else if (lcTypeStr == TEXT("strength"))
		{
			return EStatClass::Strength;
		}
		else if (lcTypeStr == TEXT("defence"))
		{
			return EStatClass::Defence;
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

			case EStatClass::Morale:
				return "morale";

			case EStatClass::Energy:
				return "energy";

			case EStatClass::Strength:
				return "strength";

			case EStatClass::Defence:
				return "defence";

			default:
				return "undefined";
		}
	}
}