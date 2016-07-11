#pragma once

//#include "SignalsBattleMode.h"

// Task method typedef.
class ASignalsBattleMode;
typedef void (ASignalsBattleMode::*TaskFunc)(class BattleTask * task);

class BattleTask
{
public:
	/**
	 * Construct the task from a method to call, and a lifetime for the
	 * task (0 = infinite) in turns.
	 */
	BattleTask(TaskFunc taskFunc, int numTurns);

	virtual ~BattleTask() = default;

	/**
	 * Can the task run this turn?
	 */
	virtual bool CanRun(ASignalsBattleMode * battle) const;

	/**
	 * Runs the task, returns true if the task should run again or false
	 * if it should be removed. This base class runs for numTurns if > 0,
	 * and forever if numTurns == 0.
	 */
	virtual bool Run(ASignalsBattleMode * battle);

protected:
	virtual void runInner(ASignalsBattleMode * battle);

private:
	TaskFunc _taskFunc;
	int _numTurns;
};
