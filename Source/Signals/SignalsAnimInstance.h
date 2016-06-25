// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Animation/AnimInstance.h"
#include "SignalsAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Transient,Blueprintable)
class SIGNALS_API USignalsAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()	
public:
	// The name of the next animatuion to trigger.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle state")
	FString NextAnimation;

	// The audio to play, if not null.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle state")
	USoundWave * Audio;

	// If true, notify the battle of animation completing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle state")
	bool NotifyCompletion;

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionStart();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionEnd();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void DeliverPayload();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void PlayAudio();
};
