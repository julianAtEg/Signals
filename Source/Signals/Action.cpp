#include "Signals.h"
#include "Action.h"
#include "XmlParser.h"

//-----------------------------------------------------------------------------

static ActionNode * createNode(FString const & type);
static TMap<FString, Action *> s_actions;

//-----------------------------------------------------------------------------

static TMap<FString, ActionNode::Ctor> & ctorTable()
{
	static TMap<FString, ActionNode::Ctor> table;
	return table;
}

/*static*/
ActionNode::Ctor ActionNode::RegisterCtor(FString const & name, Ctor ctor)
{
	UE_LOG(SignalsLog, Log, TEXT("Registering ActionNode ctor for type '%s'"), *name);

	ctorTable().Add(name, ctor);
	return ctor;
}

ActionNode::ActionNode(FString const & type)
: _type(type)
{

}

ActionNode::~ActionNode()
{
	for (auto & child : _children)
	{
		delete child;
	}
}

void ActionNode::FromXml(FXmlNode const * node)
{
	for (auto child = node->GetFirstChildNode(); child != nullptr; child = child->GetNextNode())
	{
		auto type = child->GetTag();
		auto childAction = createNode(type);
		_children.Add(childAction);
		childAction->FromXml(child);			
	}
}

ActionNode * ActionNode::FindChildOfType(FString const & type)
{
	// SHALLOW search!
	for( auto & child : _children)
	{
		if (child->GetType() == type)
		{
			return child;
		}
	}

	return nullptr;
}

void ActionNode::Execute(UWorld * world, Combatant * source, TArray<Combatant *> const & targets)
{
	for (auto & child : _children)
	{
		child->Execute(world, source, targets);
	}
}

void ActionNode::postInitialize(Action * action)
{
}

//-----------------------------------------------------------------------------

/*static*/
void Action::Initialize()
{
	UE_LOG(SignalsLog, Log, TEXT("Initializing action table"));

	auto contentFolder = FPaths::GameContentDir();
	auto actionsPath = FPaths::Combine(*contentFolder, TEXT("Data/actions.xml"));
	UE_LOG(SignalsLog, Log, TEXT("Actions data path = %s"), *actionsPath);
	FXmlFile actionsXml;
	if (!actionsXml.LoadFile(actionsPath, EConstructMethod::Type::ConstructFromFile))
	{
		UE_LOG(SignalsLog, Error, TEXT("Error loading actions.xml"));
	}

	auto root = actionsXml.GetRootNode();
	auto & children = root->GetChildrenNodes();
	for (auto actionNode : children)
	{
		auto action = new Action();
		action->FromXml(actionNode);
		UE_LOG(SignalsLog, Log, TEXT("Read action '%s'"), *action->GetName());
		s_actions.Add(action->GetName(), action);
	}
}

/*static*/
Action * Action::FindAction(FString const & name)
{
	auto ret = s_actions.Find(name);
	return(ret != nullptr ? *ret : nullptr);
}

Action::Action()
: _id(0)
, _name(TEXT("???"))
, _category(TEXT("???"))
, _cost(0)
, _warmup(0)
, _affectsMultipleTargets(false)
, _menuIcon(0)
, _root(TEXT("action"))
, _warmupNode(nullptr)
, _activityNode(nullptr)
, _payloadNode(nullptr)
{

}

Action::~Action()
{
}

void Action::FromXml(FXmlNode const * node)
{
	// Read my attributes.
	auto idStr = node->GetAttribute(TEXT("id"));
	_id = FCString::Atoi(*idStr);
	_name = node->GetAttribute(TEXT("name"));
	_category = node->GetAttribute(TEXT("category"));
	auto costStr = node->GetAttribute(TEXT("cost"));
	_cost = FCString::Atoi(*costStr);
	auto targetStr = node->GetAttribute(TEXT("target"));
	_affectsMultipleTargets = (targetStr == TEXT("multiple"));
	_description = node->GetAttribute(TEXT("description"));
	auto iconStr = node->GetAttribute(TEXT("menuIcon"));
	_menuIcon = FCString::Atoi(*iconStr);

	// Read child attributes.
	_root.FromXml(node);

	// Lashup of actions.
	_warmupNode = _root.FindChildOfType(TEXT("warmup"));
	if (_warmupNode != nullptr)
	{
		_warmupNode->postInitialize(this);
	}

	_activityNode = _root.FindChildOfType(TEXT("activity"));
	check(_activityNode != nullptr);
	_activityNode->postInitialize(this);

	_payloadNode = _root.FindChildOfType(TEXT("payload"));
	check(_payloadNode != nullptr);
	_payloadNode->postInitialize(this);
}

void Action::RunWarmup(UWorld * world, Combatant * source, TArray<Combatant *> const & targets)
{
	if (_warmupNode != nullptr)
	{
		_warmupNode->Execute(world, source, targets);
	}
}

void Action::RunActivity(UWorld * world, Combatant * source, TArray<Combatant *> const & targets)
{
	check(_activityNode != nullptr);

	_activityNode->Execute(world, source, targets);
}

void Action::RunPayload(UWorld * world, Combatant * source, TArray<Combatant *> const & targets)
{
	check(_payloadNode != nullptr);

	_payloadNode->Execute(world, source, targets);	
}

void Action::SetWarmupRounds(int rounds)
{
	check(rounds >= 0);

	_warmup = rounds;
}

//-----------------------------------------------------------------------------

ActionInstance::ActionInstance(Action * action, Combatant * source, TArray<Combatant *> const & targets)
: _action( action )
, _source( source )
, _targets( targets )
{
}

void ActionInstance::RunWarmup(UWorld * world)
{
	_action->RunWarmup(world, _source, _targets);
}

void ActionInstance::RunActivity(UWorld * world)
{
	_action->RunActivity(world, _source, _targets);
}

void ActionInstance::RunPayload(UWorld * world)
{
	_action->RunPayload(world, _source, _targets);
}

//-----------------------------------------------------------------------------

static ActionNode * createNode(FString const & type)
{
	ActionNode::Ctor * pctor = ctorTable().Find(type);
	if (pctor == nullptr)
	{
		return new ActionNode(type);
	}
	else
	{
		ActionNode::Ctor ctor = *pctor;
		return ctor();
	}
}