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
, _tolerance(100.0f)
, _command(WalkCommand::StayPut)
{

}

bool WalkNode::Update(ASignalsBattleMode * const, float)
{
	return(_command == WalkCommand::StayPut);
}

void WalkNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto commStr = node->GetAttribute(TEXT("to"));
	if (commStr == TEXT("target"))
	{
		_command = WalkCommand::WalkToTarget;
	}
	else if (commStr == "home")
	{
		_command = WalkCommand::WalkToHome;
	}

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
	auto ai = Cast<AAIController>(walkerAvatar->GetController());
	switch (_command)
	{
		case WalkCommand::WalkToTarget:
		{
			auto targets = battle->GetActionTargets();
			ai->MoveToActor(targets[0]->Avatar, _tolerance, false, false, false);
			break;
		}

		case WalkCommand::WalkToHome:
		{
			auto start = walker->Start;
			ai->MoveToActor(start, 5.0f, false, false, false);
			break;
		}
	}
}
