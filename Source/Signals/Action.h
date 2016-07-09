#pragma once

#include "ContainerNode.h"
#include "ActionClass.h"
#include "StatType.h"

class Action
{
public:
	Action();
	~Action();

	// Read from config.
	void FromXml(FXmlNode * const node);

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

	// Get / set the class of the action.
	EActionClass GetClass() const;
	void SetClass(EActionClass actionClass);

	// Get / set what the action is directed at.
	EStatClass GetTargets() const;
	void AddTarget(EStatClass target);

	// For the AI, the score measures some kind of expected payoff value - action dependent.
	int GetScore() const;
	void SetScore( int score );

	// Gets the warmup / action / payload nodes.
	ContainerNode * GetWarmupNode() const;
	ContainerNode * GetActivityNode() const;
	ContainerNode * GetPayloadNode() const;

	// Loads resources for the action.
	void LoadResources(ASignalsBattleMode * const battle);

	// Initialize / terminate the action system (one-time at startup/shutdown)
	static void Initialize();
	static void Terminate();

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

	// The expected payoff value of the action.
	int _score;

	// The number of rounds of warmup the action takes in battle.
	int _warmup;

	// Does it affect single or multiple targets?
	bool _affectsMultipleTargets;

	// Index of the action's menu icon (see the UI Blueprint).
	int _menuIcon;

	// The type of the action, and what traits it's directed at.
	EActionClass _class;
	EStatClass _targets;

	// Main action nodes.
	ContainerNode _root;
	ContainerNode * _warmupNode;
	ContainerNode * _activityNode;
	ContainerNode * _payloadNode;

	// Resources loaded?
	bool _loaded;
};

inline EActionClass Action::GetClass() const
{
	return _class;
}

inline EStatClass Action::GetTargets() const
{
	return _targets;
}

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

inline int Action::GetScore() const
{
	return _score;
}

inline int Action::GetWarmupRounds() const
{
	return _warmup;
}

inline bool Action::AffectsMultipleTargets() const
{
	return _affectsMultipleTargets;
}

inline ContainerNode * Action::GetWarmupNode() const
{
	return _warmupNode;
}

inline ContainerNode * Action::GetActivityNode() const
{
	return _activityNode;
}

inline ContainerNode * Action::GetPayloadNode() const
{
	return _payloadNode;
}
