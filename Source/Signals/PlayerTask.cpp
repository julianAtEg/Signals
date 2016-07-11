#include "Signals.h"
#include "PlayerTask.h"
#include "SignalsBattleMode.h"

PlayerTask::PlayerTask(TaskFunc func, int numTurns, FString const & playerName)
: BattleTask(func, numTurns)
, _playerName(playerName)
{
}

bool PlayerTask::CanRun(ASignalsBattleMode * battle) const
{
	return(battle->GetActiveCombatant() == _playerName);
}