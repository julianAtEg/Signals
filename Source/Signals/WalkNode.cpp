#include "Signals.h"
#include "WalkNode.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new WalkNode();
}

static const FString s_type(TEXT("walk"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

WalkNode::WalkNode()
: ActionNode(s_type)
{

}

bool WalkNode::Update(ASignalsBattleMode * const, float)
{
	return(false);
}

void WalkNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto proxStr = node->GetAttribute(TEXT("proximity"));
	if (!proxStr.IsEmpty())
	{
		_tolerance = FCString::Atof(*proxStr);
	}
}

void WalkNode::executeInner( ASignalsBattleMode * const battle, Combatant * walker )
{
	UE_LOG(SignalsLog, Log, TEXT("WalkNode::executeInner()"));

	// Warning: confusing terminology here as "target" is actually the walker
	auto walkerAvatar = walker->Avatar;
	auto walkerPos = walkerAvatar->GetActorLocation();
	auto ai = Cast<AAIController>(walkerAvatar->GetController());
	auto targets = battle->GetActionTargets();
	ai->MoveToActor(targets[0]->Avatar, _tolerance, false, false, false);
	walker->HasMoved = true;
}
