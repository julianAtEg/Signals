// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "Combatant.h"

void Combatant::OnTurnBeginning()
{
	HPDamageThisTurn = 0;
	ActionMissed = false;
}