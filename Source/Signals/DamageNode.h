#pragma once

#include "ActionNode.h"

enum class DamageType
{
	// ???
	Undefined,

	// Damages HP
	HitPoints,

	// Damages morale / speed
	Morale,

	// Affects energy level
	Energy,

	// Affects strength
	Strength,

	// Affects defence
	Defence,
};

class DamageNode : public ActionNode
{
public:
	DamageNode();

	void FromXml(FXmlNode * const node) override;
	void Execute(ASignalsBattleMode * const battle) override;

private:
	// The type of damage delivered.
	DamageType _type;

	// Damage formulae parameters.
	int _base;
	int _levelScale;
	int _min;
	int _max;

	// If true, the damage persists after a battle.
	bool _persistent;
};
