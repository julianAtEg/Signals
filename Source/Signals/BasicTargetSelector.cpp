#include "Signals.h"
#include "BasicTargetSelector.h"
#include "Action.h"
#include "SignalsBattleMode.h"
#include "Random.h"
#include "NpcPlayerStats.h"
#include "ActionClass.h"

//-----------------------------------------------------------------------------

static BasicTargetSelector s_instance;
static TargetSelectionPolicy * s_policy = BasicStrategy::RegisterTargetSelector(TEXT("basic"), &s_instance);

//-----------------------------------------------------------------------------

static Combatant * findWeakestPlayer(TArray<Combatant *> const & players);

//-----------------------------------------------------------------------------

bool BasicTargetSelector::Run(BasicStrategy * strategy, ASignalsBattleMode * battle, Combatant * npc)
{
	auto rng = battle->GetRandom();
	auto action = strategy->GetAction();
	check(action != nullptr);
	if (action->GetClass() == EActionClass::Offensive)
	{
		auto enemies = battle->GetActiveHumans();
		if (action->AffectsMultipleTargets())
		{
			strategy->SetTargets(enemies);
		}
		else
		{
			auto stats = Cast<UNpcPlayerStats>(npc->Stats);
			// The higher the callousness, the more likely they go for the weakest player.
			if (rng->NextInt(0, 100) > stats->Callousness)
			{
				// TODO: expand "weakness" to include the target class of the action.
				auto target = findWeakestPlayer(enemies);
				strategy->SetTarget(target);
			}
			else
			{
				// Pick a random target.
				int index = rng->NextInt(0, enemies.Num() - 1);
				strategy->SetTarget(enemies[index]);
			}
		}
	}
	else
	{
		// Restorative / neutral / defensive action.
		auto friends = battle->GetActiveNPCs();
		if (action->AffectsMultipleTargets())
		{
			strategy->SetTargets(friends);
		}
		else
		{
			// Pick a random target.
			int index = rng->NextInt(0, friends.Num() - 1);
			strategy->SetTarget(friends[index]);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------

static Combatant * findWeakestPlayer(TArray<Combatant *> const & players)
{
	int minHP = 0xFFFFFFFF;
	Combatant * runt = nullptr;
	for (auto & player : players)
	{
		if (player->Stats->HitPoints < minHP)
		{
			runt = player;
			minHP = player->Stats->HitPoints;
		}
	}

	return runt;
}
