#pragma once

#include "ActionNode.h"

enum FaceCommand
{
	// Keep facing the way you're facing.
	FaceCurrent,

	// Face the command target.
	FaceTarget,

	// Face a point in world space.
	FacePoint,

	// TODO: face a general actor in the world.
	FaceActor,
};

class FaceNode : public ActionNode
{
public:
	FaceNode();

	void FromXml(FXmlNode * node) override;
	bool Update(ASignalsBattleMode * const battle, float dt) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * player) override;

private:
	FaceCommand _command;
	FVector _point;
	FRotator _targetRot;
	ACharacter * _rotator;
};
