#include "Signals.h"
#include "PlayerStatus.h"
#include "PlayerStats.h"
#include "Combatant.h"
#include "SignalsBattleMode.h"
#include <map>

static std::map<FString, EPlayerStatus> s_map =
{
	{ TEXT("deranged"), EPlayerStatus::Deranged },
	{ TEXT("sick"), EPlayerStatus::Sick },
	{ TEXT("slow"), EPlayerStatus::Slow },
	{ TEXT("fast"), EPlayerStatus::Fast },
	{ TEXT("sloppy"), EPlayerStatus::Sloppy },
	{ TEXT("sharp"), EPlayerStatus::Sharp },
	{ TEXT("weak"), EPlayerStatus::Weak },
	{ TEXT("titan"), EPlayerStatus::Titan },
	{ TEXT("paralysed"), EPlayerStatus::Paralyzed },
};

namespace PlayerStatus
{
	EPlayerStatus FromString(FString const & str)
	{
		auto lc = str.ToLower();
		return s_map.at(lc);
	}

	FString ToString(EPlayerStatus status)
	{
		for (auto & kv : s_map)
		{
			if (kv.second == status)
			{
				return(kv.first);
			}
		}

		return(TEXT("???"));
	}

	bool AffectsSchedule(EPlayerStatus status)
	{
		switch (status)
		{
		case EPlayerStatus::Fast:
		case EPlayerStatus::Slow:
			return true;

		default:
			return false;
		}
	}

	void Apply(EPlayerStatus status, Combatant * player)
	{
		switch (status)
		{
		case EPlayerStatus::Fast:
			player->Stats->ApplyStatChange(EStatClass::Speed, 5);
			break;

		case EPlayerStatus::Slow:
			player->Stats->ApplyStatChange(EStatClass::Speed, -5);
			break;

		default:
			break;
		}
	}

	void Remove(EPlayerStatus status, Combatant * player)
	{
		switch (status)
		{
		case EPlayerStatus::Fast:
			player->Stats->ApplyStatChange(EStatClass::Speed, -5);
			break;

		case EPlayerStatus::Slow:
			player->Stats->ApplyStatChange(EStatClass::Speed, 5);
			break;

		default:
			break;
		}
	}
}