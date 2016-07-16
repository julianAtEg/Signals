#pragma once

#include "TaskSchedule.h"

class ASignalsBattleMode;
typedef Task<ASignalsBattleMode> BattleTask;

//class BattleTask : public Task
//{
//public:
//	/**
//	* Runs the task, returns true if the task should run again or false
//	* if it should be removed.
//	*/
//	virtual bool Run(ASignalsBattleMode * battle) = 0;
//
//protected:
//	BattleTask(int interval = 1, int iterations = -1);
//};

typedef TaskSchedule<BattleTask> BattleSchedule;
