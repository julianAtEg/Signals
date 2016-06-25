#include "Signals.h"
#include "FaceNode.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new FaceNode(); }
static const FString s_type(TEXT("face"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

static const float TURN_RATE = 140.0f;

//-----------------------------------------------------------------------------

FaceNode::FaceNode()
: ActionNode(s_type)
, _command(FaceCommand::FaceCurrent)
, _point()
{
}

void FaceNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto dirStr = node->GetAttribute(TEXT("direction"));
	if (dirStr == TEXT("target"))
	{
		_command = FaceCommand::FaceTarget;
	}
	else if (dirStr == TEXT("point"))
	{
		_command = FaceCommand::FacePoint;
		auto ptStr = node->GetAttribute(TEXT("point"));
		if (ptStr.IsEmpty())
		{
			UE_LOG(SignalsLog, Warning, TEXT("Missing target point in FaceNode"));
		}
		else
		{
			TArray<FString> comps;
			int len = ptStr.ParseIntoArray(comps, TEXT(","));
			if (len != 3)
			{
				UE_LOG(SignalsLog, Warning, TEXT("Invalid target point in FaceNode"));
			}
			else
			{
				_point.X = FCString::Atof(*comps[0]);
				_point.Y = FCString::Atof(*comps[1]);
				_point.Z = 0.0f;// FCString::Atof(*comps[2]);
			}
		}
	}
}

bool FaceNode::Update(ASignalsBattleMode * const, float dt)
{
	bool finished = false;
	auto currentRot = _rotator->GetActorRotation();
	if (currentRot.Yaw < _targetRot.Yaw)
	{
		currentRot.Yaw += TURN_RATE*dt;
		if (currentRot.Yaw >= _targetRot.Yaw)
		{
			currentRot.Yaw = _targetRot.Yaw;
			finished = true;
		}
	}
	else if (currentRot.Yaw > _targetRot.Yaw)
	{
		currentRot.Yaw -= TURN_RATE*dt;
		if (currentRot.Yaw <= _targetRot.Yaw)
		{
			currentRot.Yaw = _targetRot.Yaw;
			finished = true;
		}
	}

	_rotator->SetActorRotation(currentRot);

	return finished;
}

void FaceNode::executeInner(ASignalsBattleMode * const battle, Combatant * actor)
{
	switch (_command)
	{
		case FaceCommand::FacePoint:
			break;

		case FaceCommand::FaceTarget:
		{
			auto & targets = battle->GetActionTargets();
			auto target = targets[0];
			_point = target->Avatar->GetActorLocation();
			break;
		}
	}

	_rotator = actor->Avatar;

	// Compute the target direction.
	auto dir = _point - actor->Avatar->GetActorLocation();
	dir.Z = 0;
	dir.Normalize();
	_targetRot = dir.Rotation();
}