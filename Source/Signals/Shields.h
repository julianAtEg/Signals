#pragma once

#include "AttackClass.h"

class ASignalsBattleMode;
struct Combatant;

namespace Shields
{
	void ShowShield(ASignalsBattleMode * battle, Combatant * player, EAttackClass type);
}
