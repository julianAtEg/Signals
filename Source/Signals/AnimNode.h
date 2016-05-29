#pragma once

#include "Action.h"

enum class AnimDestination
{
	DontPlay,
	PlayOnSource,
	PlayOnTargets
};

class AnimNode : public ActionNode
{
public:
	AnimNode();

	void FromXml(FXmlNode const * node) override;
	void Execute(UWorld * world, Combatant * source, TArray<Combatant *> const & targets) override;

	// Gets where an animation is to be played - on the source or the targets.
	AnimDestination GetDestination() const;

private:
	FString _anim;
	AnimDestination _destination;
};

