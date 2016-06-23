#include "Signals.h"
#include "Action.h"
#include "Combatant.h"
#include "XmlParser.h"

//-----------------------------------------------------------------------------

static TMap<FString, Action *> s_actions;

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
, _score(0)
, _warmup(0)
, _affectsMultipleTargets(false)
, _menuIcon(0)
, _class(EActionClass::Neutral)
, _targets(EStatClass::Undefined)
, _root(TEXT("action"))
, _warmupNode(nullptr)
, _activityNode(nullptr)
, _payloadNode(nullptr)
{

}

Action::~Action()
{
}

void Action::SetScore(int score)
{
	_score = score;
}

void Action::LoadResources(ASignalsBattleMode * const battle)
{
	_root.LoadResources(battle);
}

void Action::FromXml(FXmlNode * const node)
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
	_warmupNode = (ContainerNode *)_root.FindChildOfType(TEXT("warmup"));
	if (_warmupNode != nullptr)
	{
		_warmupNode->PostInitialize(this);
	}

	_activityNode = (ContainerNode *)_root.FindChildOfType(TEXT("activity"));
	check(_activityNode != nullptr);
	_activityNode->PostInitialize(this);

	_payloadNode = (ContainerNode  *)_root.FindChildOfType(TEXT("payload"));
	check(_payloadNode != nullptr);
	_payloadNode->PostInitialize(this);
}

void Action::SetWarmupRounds(int rounds)
{
	check(rounds >= 0);

	_warmup = rounds;
}

void Action::SetClass(EActionClass actionClass)
{
	_class = actionClass;
}

void Action::AddTarget(EStatClass target)
{
	_targets = (EStatClass)(_targets | target);
}