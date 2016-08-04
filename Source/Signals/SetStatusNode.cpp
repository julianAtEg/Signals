#include "Signals.h"
#include "SetStatusNode.h"
#include "Combatant.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

REGISTER_NODE_TYPE("set-status", SetStatusNode);

//-----------------------------------------------------------------------------

namespace
{
	struct UndoStatusTask : public PlayerTask
	{
		UndoStatusTask(ASignalsBattleMode * battle, EPlayerStatus status, int interval)
		: PlayerTask(interval, 1)
		, Battle(battle)
		, Status(status)
		{

		}

		bool Run(Combatant * player) override
		{
			player->ClearStatus(Status);
			if (PlayerStatus::AffectsSchedule(Status))
			{
				Battle->ReschedulePlayer(player);
			}
			return true;
		}

		ASignalsBattleMode * const Battle;
		EPlayerStatus const Status;
	};
}
//-----------------------------------------------------------------------------

SetStatusNode::SetStatusNode()
: ActionNode(s_type)
, _status(EPlayerStatus::NumStatusTypes)
{

}

void SetStatusNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto statusStr = node->GetAttribute(TEXT("status"));
	_status = PlayerStatus::FromString(statusStr);
	auto durStr = node->GetAttribute(TEXT("duration"));
	_duration = ActionDuration::FromString(durStr);
	if (_duration == EDuration::dtTurns)
	{
		auto turnsStr = node->GetAttribute(TEXT("turns"));
		_turns = FCString::Atoi(*turnsStr);
	}
	else if (_duration == EDuration::dtBattle)
	{
		_turns = INT_MAX;
	}
}

void SetStatusNode::executeInner(ASignalsBattleMode *battle, Combatant * target)
{
	target->SetStatus(_status);
	if (PlayerStatus::AffectsSchedule(_status))
	{
		battle->ReschedulePlayer(target);
	}

	target->AddTask(new UndoStatusTask(battle, _status, _turns));
}