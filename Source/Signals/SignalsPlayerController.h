// (c) Smoke and Mirrors Software 2016

#pragma once

#include "GameFramework/PlayerController.h"
#include "SignalsPlayerController.generated.h"

/**
*
*/
UCLASS()
class SIGNALS_API ASignalsPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetupInputComponent() override;



};
