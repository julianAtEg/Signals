#include "Signals.h"
#include "BattleTask.h"

BattleTask::BattleTask(TaskFunc taskFunc, int numTurns)
: _taskFunc( taskFunc )
, _numTurns( numTurns )
{
	check(taskFunc != nullptr);
}

bool BattleTask::Run(ASignalsBattleMode * battle)
{
	runInner(battle);
	if (_numTurns > 0)
	{
		--_numTurns;
		if (_numTurns == 0)
		{
			return false;
		}
	}
	return true;
}

void BattleTask::runInner(ASignalsBattleMode * battle)
{
	(battle->*_taskFunc)(this);
}

bool BattleTask::CanRun(ASignalsBattleMode *) const
{
	return true;
}