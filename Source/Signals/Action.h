#pragma once

#include "Engine.h"
#include "XmlParser.h"

struct Combatant;
class UWorld;
class ActionNode;

class ActionNode
{
	friend class Action;
public:
	// Construct a node of the supplied type.
	ActionNode(FString const & type);
	virtual ~ActionNode();

	// Reads the node properties from an Xml archive.
	virtual void FromXml(FXmlNode const * node);

	// Finds a child node of the given type (shallow).
	ActionNode * FindChildOfType(FString const & name);

	// Gets the type of this node.
	FString const & GetType() const;

	// Performs some action.
	virtual void Execute(UWorld * world, Combatant * source, TArray<Combatant *> const & targets);

	// Factory interface.
	typedef ActionNode * (*Ctor)();
	static Ctor RegisterCtor(FString const & type, Ctor ctor);

private:
	// Called after node creation to modify the action appropriately.
	virtual void postInitialize(Action * action);

	FString _type;
	TArray<ActionNode *> _children;
};

inline FString const & ActionNode::GetType() const
{
	return _type;
}

//-----------------------------------------------------------------------------

class Action
{
	friend class ActionInstance;
public:
	Action();
	~Action();

	// Read from config.
	void FromXml(FXmlNode const * node);

	// Gets the unique ID of the action.
	int GetID() const;

	// Gets the name of the action.
	FString const & GetName() const;

	// Gets the category of the action.
	FString const & GetCategory() const;

	// Gets a versbose description of the action.
	FString const & GetDescription() const;

	// Gets the cost of the action.
	int GetCost() const;

	// Does it affect multiple targets?
	bool AffectsMultipleTargets() const;

	// Gets/sets the warmup time (#rounds)
	int GetWarmupRounds() const;
	void SetWarmupRounds(int rounds);

	// Gets the index of the icon to show in menus.
	int GetMenuIcon() const;

	// Run the warmup / main activity / payload.
	void RunWarmup(UWorld * world, Combatant * source, TArray<Combatant *> const & targets);
	void RunActivity(UWorld * world, Combatant * source, TArray<Combatant *> const & targets);
	void RunPayload(UWorld * world, Combatant * source, TArray<Combatant *> const & targets);

	// Initialize the action system (one-time at startup)
	static void Initialize();

	// Find an action by name.
	static Action * FindAction(FString const & name);

private:
	// The ID of the action.
	int _id;

	// The name of the action.
	FString _name;

	// Gets a verbose description of the action.
	FString _description;

	// What category the action falls into.
	FString _category;

	// The cost in erg of the action.
	int _cost;

	// The number of rounds of warmup the action takes in battle.
	int _warmup;

	// Does it affect single or multiple targets?
	bool _affectsMultipleTargets;

	// Index of the action's menu icon.
	int _menuIcon;

	// Main action nodes.
	ActionNode _root;
	ActionNode * _warmupNode;
	ActionNode * _activityNode;
	ActionNode * _payloadNode;
};

inline int Action::GetMenuIcon() const
{
	return _menuIcon;
}

inline int Action::GetID() const
{
	return _id;
}

inline FString const & Action::GetName() const
{
	return _name;
}

inline FString const & Action::GetCategory() const
{
	return _category;
}

inline FString const & Action::GetDescription() const
{
	return _description;
}

inline int Action::GetCost() const
{
	return _cost;
}

inline int Action::GetWarmupRounds() const
{
	return _warmup;
}

inline bool Action::AffectsMultipleTargets() const
{
	return _affectsMultipleTargets;
}

//-----------------------------------------------------------------------------

class ActionInstance
{
public:
	ActionInstance(Action * action, Combatant * source, TArray<Combatant *> const & targets);

	int GetWarmupTurns() const;
	void RunWarmup(UWorld * world);
	void RunActivity(UWorld * world);
	void RunPayload(UWorld * world);
	FString const & GetName() const;

private:
	Action * const _action;
	Combatant * const _source;
	TArray<Combatant *> _targets;
};

inline FString const & ActionInstance::GetName() const
{
	return _action->GetName();
}

inline int ActionInstance::GetWarmupTurns() const
{
	return _action->GetWarmupRounds();
}