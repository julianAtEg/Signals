// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SignalsGameMode.generated.h"

class UInputComponent;
class UUserWidget;

/**
 * 
 */
UCLASS()
class SIGNALS_API ASignalsGameMode : public AGameMode
{
	GENERATED_BODY()
		
public:
	void BeginPlay() override;
	virtual void InitializeInput(UInputComponent * input);

protected:
	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> HudWidgetClass;

private:
	UPROPERTY()
	UUserWidget * _hud;
	
};
