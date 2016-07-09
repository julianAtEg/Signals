#include "Signals.h"
#include "ActionNode.h"
#include "SignalsBattleMode.h"

ActionNode::ActionNode(FString const & type, bool isContainer /*=false*/)
: IsContainer(isContainer)
, _type(type)
, _destination( Destination::None )
, _customTarget(TEXT(""))
{

}

ActionNode::~ActionNode()
{
}

void ActionNode::FromXml(FXmlNode * const node)
{
	auto destStr = node->GetAttribute(TEXT("destination"));
	destStr.ToLowerInline();
	if (destStr == TEXT("source"))
	{
		_destination = Destination::Source;
	}
	else if (destStr == TEXT("targets"))
	{
		_destination = Destination::Targets;
	}
	else if (!destStr.IsEmpty())
	{
		_destination = Destination::Custom;
		_customTarget = destStr;
	}
	else
	{
		_destination = Destination::None;
	}
}

void ActionNode::Execute(ASignalsBattleMode * battle)
{
	switch (_destination)
	{
		case Destination::Source:
		{
			auto target = battle->GetActionSource();
			executeInner(battle, target);
			break;
		}

		case Destination::Targets:
		{
			auto & targets = battle->GetActionTargets();
			for (auto target : targets)
			{
				executeInner(battle, target);
			}
		}

		case Destination::Custom:
			executeCustom(battle);
			break;

		case Destination::None:
			executeInner(battle, nullptr);
			break;
	}
}

bool ActionNode::Update(ASignalsBattleMode * const, float)
{
	return true;
}

void ActionNode::PostInitialize(Action * const)
{
}

void ActionNode::LoadResources(ASignalsBattleMode * const battle)
{

}

void ActionNode::UnloadResources(ASignalsBattleMode * const battle)
{

}

void ActionNode::executeInner(ASignalsBattleMode *, Combatant *)
{

}

void ActionNode::executeCustom(ASignalsBattleMode *)
{

}