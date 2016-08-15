#include "Signals.h"
#include "SetStatusNode.h"
#include "Combatant.h"
#include "SignalsBattleMode.h"
#include <typeinfo>

//-----------------------------------------------------------------------------

REGISTER_NODE_TYPE("set-status", SetStatusNode);

//-----------------------------------------------------------------------------

namespace
{
	struct UndoStatusTask : public PlayerTask
	{
		UndoStatusTask(ASignalsBattleMode * battle, EPlayerStatus status, int interval)
		: PlayerTask(TypeID, interval, 1)
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

		static const TaskType TypeID;
	};

	const TaskType UndoStatusTask::TypeID = (const TaskType)&UndoStatusTask::TypeID;

	struct IsStatusUndo
	{
		inline IsStatusUndo(EPlayerStatus status)
		: _status(status)
		{

		}

		inline bool operator()(TaskBase * task)
		{
			if (task->GetTypeID() != UndoStatusTask::TypeID)
				return false;

			auto undo = static_cast<UndoStatusTask *>(task);
			return(undo->Status == _status);
		}

	private:
		EPlayerStatus _status;
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
	if (!target->HasStatus(_status))
	{
		target->SetStatus(_status);
		if (PlayerStatus::AffectsSchedule(_status))
		{
			battle->ReschedulePlayer(target);
		}

		target->AddTask(new UndoStatusTask(battle, _status, _turns));
	}
	else
	{
		// Already have this status. For statuses that last a finite number of 
		// turns, find the UndoStatusTask for that status so we can extend the 
		// life of the task.
		if (_turns < INT_MAX)
		{
			auto & schedule = target->GetTaskSchedule();
			auto & tasks = schedule.GetTasks();
			auto task = tasks.FindByPredicate(IsStatusUndo(_status));
			if (task != nullptr)
			{
				// Found. Extend the lifetime.
				auto undo = static_cast<UndoStatusTask *>(*task);
				undo->Extend(_turns);
				schedule.Reschedule(undo);
			}
		}
	}
}