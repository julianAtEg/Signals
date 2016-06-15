// (c) Smoke and Mirrors Games 2016

#pragma once

#include "AIController.h"
#include "BattleAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIGNALS_API ABattleAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void OnMoveCompleted(FAIRequestID requestID, EPathFollowingResult::Type result) override;		
};
