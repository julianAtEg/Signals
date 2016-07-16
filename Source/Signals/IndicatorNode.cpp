#include "Signals.h"
#include "IndicatorNode.h"
#include "SignalsBattleMode.h"
#include "Combatant.h"

//-----------------------------------------------------------------------------
static ActionNode * ctor() { return new IndicatorNode(); }
static const FString s_type(TEXT("indicator"));
static const ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);
//-----------------------------------------------------------------------------

IndicatorNode::IndicatorNode()
: ActionNode(s_type)
, _text(TEXT("???"))
, _colour(1.0f,1.0f,1.0f)
{
}

void IndicatorNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	_text = node->GetAttribute(TEXT("text"));
	if (_text.Len() == 0)
	{
		UE_LOG(SignalsLog, Warning, TEXT("Empty string in indicator node"));
		_text = TEXT("???");
	}

	auto colorStr = node->GetAttribute(TEXT("colour"));
	if (colorStr.Len() > 0)
	{
		TArray<FString> components;
		if (colorStr.ParseIntoArray(components, TEXT(",")) == 3)
		{
			_colour.X = FCString::Atof(*components[0]);
			_colour.Y = FCString::Atof(*components[1]);
			_colour.Z = FCString::Atof(*components[2]);
		}
		else
		{
			UE_LOG(SignalsLog, Warning, TEXT("Invalid colour specifier in indicator node"));
		}
	}
}

void IndicatorNode::executeInner(ASignalsBattleMode * battle, Combatant * player)
{
	battle->AddFloatingNotification(player->Avatar, _text, _colour);
}