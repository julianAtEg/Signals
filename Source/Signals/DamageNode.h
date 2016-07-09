#pragma once

#include "ActionNode.h"
#include "ContainerNode.h"
#include "AttackClass.h"
#include "StatNode.h"

// Contains a bunch of damage nodes, the effects of which are globbed together
// before issuing notifications.
class DamageContainer : public ContainerNode
{
public:
	DamageContainer();

	void PostInitialize(Action * const action) override;
	void OnLeave(ASignalsBattleMode * const battle) override;
};

//-----------------------------------------------------------------------------

class DamageNode : public StatNode
{
public:
	DamageNode();

	void FromXml(FXmlNode * const node) override;

protected:
	void executeInner(ASignalsBattleMode * const battle, Combatant * combatant) override;

private:
	// The type of damage delivered.
	EAttackClass _class;

	// If true, damage can be avoided by a suitably agile player
	bool _isAvoidable;
};
