// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "SignalsPlayerController.h"
#include "SignalsGameMode.h"

void ASignalsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	auto mode = static_cast<ASignalsGameMode *>(GetWorld()->GetAuthGameMode());
	mode->InitializeInput(this->InputComponent);
}


