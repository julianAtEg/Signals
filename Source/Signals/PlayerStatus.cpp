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

//-----------------------------------------------------------------------------

static StatModifier * addModifier(UPlayerStats * stats, EStatClass stat, int delta)
{
	auto mod = new AddStatModifier(stat, delta);
	stats->AddStatModifier(mod);
	return mod;
}

static StatModifier * percentModifier(UPlayerStats * stats, EStatClass stat, int percent)
{
	auto mod = new PercentageStatModifier(stat, percent);
	stats->AddStatModifier(mod);
	return mod;
}
//-----------------------------------------------------------------------------

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
		case EPlayerStatus::Sick:
			return true;

		default:
			return false;
		}
	}

	TArray<StatModifier *> Apply(EPlayerStatus status, Combatant * player)
	{
		auto world = player->Avatar->GetWorld();
		auto battle = Cast<ASignalsBattleMode>(world->GetAuthGameMode());

		TArray<StatModifier *> mods;
		switch (status)
		{
		case EPlayerStatus::Fast:
			mods.Add(addModifier(player->Stats, EStatClass::Speed, 5));
			mods.Add(addModifier(player->Stats, EStatClass::Dexterity, 1));
			mods.Add(addModifier(player->Stats, EStatClass::Evasion, 1));
			break;

		case EPlayerStatus::Slow:
			mods.Add(addModifier(player->Stats, EStatClass::Speed, -5));
			mods.Add(addModifier(player->Stats, EStatClass::Dexterity, -1));
			mods.Add(addModifier(player->Stats, EStatClass::Evasion, -1));
			break;

		case EPlayerStatus::Sick:
			mods.Add(addModifier(player->Stats, EStatClass::Speed, -1));
			mods.Add(addModifier(player->Stats, EStatClass::Dexterity, -1));
			mods.Add(addModifier(player->Stats, EStatClass::Evasion, -1));
			break;

		case EPlayerStatus::Sloppy:
			mods.Add(percentModifier(player->Stats, EStatClass::Dexterity, -20));
			mods.Add(percentModifier(player->Stats, EStatClass::Evasion, -20));
			break;

		case EPlayerStatus::Sharp:
			mods.Add(percentModifier(player->Stats, EStatClass::Dexterity, 20));
			mods.Add(percentModifier(player->Stats, EStatClass::Evasion, 20));
			break;

		case EPlayerStatus::Weak:
			mods.Add(percentModifier(player->Stats, EStatClass::Strength, -20));
			mods.Add(percentModifier(player->Stats, EStatClass::PhysicalDefence, -20));
			break;

		case EPlayerStatus::Titan:
			mods.Add(percentModifier(player->Stats, EStatClass::Strength, 20));
			mods.Add(percentModifier(player->Stats, EStatClass::PhysicalDefence, 20));
			break;

		case EPlayerStatus::Paralyzed:
			battle->PlayAnimation(player->Avatar, TEXT("freeze"), nullptr, false);
			break;

		default:
			break;
		}

		return mods;
	}

	EPlayerStatus GetAntiStatus(EPlayerStatus status)
	{
		switch (status)
		{
		case EPlayerStatus::Fast:
			return EPlayerStatus::Slow;

		case EPlayerStatus::Slow:
			return EPlayerStatus::Fast;

		case EPlayerStatus::Titan:
			return EPlayerStatus::Weak;

		case EPlayerStatus::Weak:
			return EPlayerStatus::Titan;

		case EPlayerStatus::Sloppy:
			return EPlayerStatus::Sharp;

		case EPlayerStatus::Sharp:
			return EPlayerStatus::Sloppy;

		default:
			return EPlayerStatus::NumStatusTypes;
		}
	}
}
