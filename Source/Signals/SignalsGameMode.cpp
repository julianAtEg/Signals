// Fill out your copyright notice in the Description page of Project Settings.

#include "Signals.h"
#include "SignalsGameMode.h"
#include <Blueprint/UserWidget.h>

void ASignalsGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass != nullptr)
	{
		_hud = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
		if (_hud != nullptr)
		{
			_hud->AddToViewport();
		}
		else
		{
			UE_LOG(SignalsLog, Error, TEXT("Could not create HUD"));
		}
	}
}

void ASignalsGameMode::InitializeInput(UInputComponent * input)
{

}




