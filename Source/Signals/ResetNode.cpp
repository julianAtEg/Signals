#include "Signals.h"
#include "ResetNode.h"
#include "Combatant.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new ResetNode(); }
static const FString s_type("reset");
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

ResetNode::ResetNode()
: ActionNode(s_type)
{

}

void ResetNode::executeInner(ASignalsBattleMode * battle, Combatant * target)
{
	UE_LOG(SignalsLog, Log, TEXT("Resetting position and rotation of '%s'"), *target->Avatar->GetName());

	auto pos = target->Start->GetActorLocation();
	auto rot = target->Start->GetActorRotation();
	target->Avatar->SetActorLocation(pos);
	target->Avatar->SetActorRotation(rot);
}