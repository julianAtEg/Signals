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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle state")
	FString NextAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle state")
	USoundWave * Audio;

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionStart();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionEnd();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void DeliverPayload();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void PlayAudio();
};
