#include "Signals.h"
#include "ShieldNode.h"
#include "Combatant.h"
#include "SignalsBattleMode.h"
#include "Shields.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new ShieldNode(); }
static const FString s_type(TEXT("shield"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

ShieldNode::ShieldNode()
: ActionNode(s_type)
, _class(EAttackClass::Physical)
, _duration(1)
{

}

void ShieldNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto classStr = node->GetAttribute(TEXT("type"));
	_class = AttackClass::FromString(classStr);

	auto durationStr = node->GetAttribute(TEXT("duration"));
	_duration = FCString::Atoi(*durationStr);
}

void ShieldNode::executeInner(ASignalsBattleMode * battle, Combatant * player)
{
	player->Shields.ActivateShield(_class, _duration);
	Shields::ShowShield(battle, player, _class);
}
