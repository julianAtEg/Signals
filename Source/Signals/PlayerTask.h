#pragma once

#include "BattleTask.h"

class PlayerTask : public BattleTask
{
public:
	PlayerTask(TaskFunc func, int numTurns, FString const & playerName);

	bool CanRun(ASignalsBattleMode * battle) const override;

private:
	FString const _playerName;
};
