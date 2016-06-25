// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "SignalsAnimInstance.h"
#include "SignalsBattleMode.h"

USignalsAnimInstance::USignalsAnimInstance(FObjectInitializer const & init)
	: Super(init)
	, NextAnimation(TEXT(""))
	, Audio(nullptr)
	, NotifyCompletion(false)
{

}

void USignalsAnimInstance::OnActionStart()
{
	// Ensure the animation doesn't re-trigger.
	UE_LOG(SignalsLog, Log, TEXT("Action %s starting"), *NextAnimation);

	NextAnimation = TEXT("");
}

void USignalsAnimInstance::OnActionEnd()
{
	UE_LOG(SignalsLog, Log, TEXT("Action ending"));

	if (NotifyCompletion)
	{
		auto world = GetWorld();
		auto mode = world->GetAuthGameMode();
		auto battle = Cast<ASignalsBattleMode>(mode);
		battle->OnActionComplete();
	}
}

void USignalsAnimInstance::DeliverPayload()
{
	UE_LOG(SignalsLog, Log, TEXT("Deliver payload"));
	auto world = GetWorld();
	auto mode = world->GetAuthGameMode();
	auto battle = Cast<ASignalsBattleMode>(mode);
	battle->RunActionPayload();
}

void USignalsAnimInstance::PlayAudio()
{
	if (Audio != nullptr)
	{
		auto world = GetWorld();
		auto object = GetOwningActor();
		auto position = object->GetActorLocation();
		auto rotation = object->GetActorRotation();
		UGameplayStatics::PlaySoundAtLocation(world, Audio, position, rotation);
	}
}
