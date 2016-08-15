#include "Signals.h"
#include "ClearStatusNode.h"
#include "Combatant.h"
#include "PlayerStats.h"

//-----------------------------------------------------------------------------

REGISTER_NODE_TYPE("clear-status", ClearStatusNode);

//-----------------------------------------------------------------------------

ClearStatusNode::ClearStatusNode()
: ActionNode(s_type)
, _statuses()
{
}

void ClearStatusNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto statuses = node->GetAttribute(TEXT("statuses"));
	TArray<FString> statusList;
	if (statuses == TEXT("*"))
	{
		for (int i = 0; i < (int)EPlayerStatus::NumStatusTypes; ++i)
		{
			_statuses.Add(EPlayerStatus(i));
		}
	}
	else
	{
		statuses.ParseIntoArray(statusList, TEXT(","));
		for (auto & strStatus : statusList)
		{
			auto status = PlayerStatus::FromString(strStatus);
			_statuses.Add(status);
		}
	}
}

void ClearStatusNode::executeInner(ASignalsBattleMode *, Combatant * player)
{
	for (auto & status : _statuses)
	{
		player->ClearStatus(status);
	}
}