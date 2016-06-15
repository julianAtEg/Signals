#include "Signals.h"
#include "EffectNode.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new EffectNode();
}

static const FString s_type(TEXT("effect"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

EffectNode::EffectNode()
: ActionNode(s_type)
, _effect(TEXT("???"))
, _sourceSocket(TEXT(""))
, _targetSocket(TEXT(""))
, _particles(nullptr)
{

}

EffectNode::~EffectNode()
{
	if (_particles != nullptr)
	{
		if (_particles->IsValidLowLevel())
		{
			_particles->ConditionalBeginDestroy();
			_particles = nullptr;
		}
	}
}

void EffectNode::FromXml(FXmlNode * const node)
{
	_effect = node->GetAttribute(TEXT("name"));
	_sourceSocket = node->GetAttribute(TEXT("sourceSocket"));
	_targetSocket = node->GetAttribute(TEXT("targetSocket"));
	ActionNode::FromXml(node);
}

void EffectNode::PostInitialize(Action * const)
{
	auto name = FString::Printf(TEXT("ParticleSystem'/Game/Particles/%s.%s'"), *_effect, *_effect);
	_particles = (UParticleSystem *)LoadObject<UParticleSystem>(nullptr, *name, nullptr, LOAD_None, nullptr);
	check(_particles != nullptr);
	_particles->AddToRoot();
}

void EffectNode::executeInner(ASignalsBattleMode * const battle, Combatant * target)
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger effect '%s'"), *_effect);

	FName socketName(*_targetSocket);
	auto position = target->Avatar->GetMesh()->GetSocketTransform(socketName);
	UGameplayStatics::SpawnEmitterAtLocation(battle->GetWorld(), _particles, position, true);
}
