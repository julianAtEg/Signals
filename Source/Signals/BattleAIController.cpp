// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "BattleAIController.h"
#include "SignalsBattleMode.h"

void ABattleAIController::OnMoveCompleted(FAIRequestID requestID, EPathFollowingResult::Type result) 
{
	UE_LOG(SignalsLog, Log, TEXT("ABattleAIController::OnMoveComplete()"));

	auto world = GetWorld();
	if (world != nullptr)
	{
		auto battle = Cast<ASignalsBattleMode>(world->GetAuthGameMode());
		if (battle != nullptr)
		{
			battle->OnActionComplete();
		}
	}
}



