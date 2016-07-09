// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "Combatant.h"
#include "Action.h"
#include "PlayerStats.h"

void Combatant::OnTurnBeginning()
{
	HPDamageThisTurn = 0;
	ActionMissed = false;
	TookDamage = false;
}

bool Combatant::CanPerformAction(Action * const action) const
{
	// TODO: implement me. 
	// if (player is Human && action->Cost < gameInstance->Ergs) return true;
	// if player is npc && action->Cost < stats->Ergs) return true;
	return true;
}

void Combatant::BeginBattle()
{
	Stats->BeginBattle();
}

void Combatant::EndBattle()
{
	Stats->EndBattle();
}
