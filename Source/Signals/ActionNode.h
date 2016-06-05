#pragma once

#include "Signals.h"
#include "XmlParser.h"

class ASignalsBattleMode;
class ActionNode;
class Action;

enum class Destination
{
	None,
	Source,
	Targets,
	Custom
};

class ActionNode
{
public:
	// Construct a node of the supplied type.
	ActionNode(FString const & type, bool isContainer = false);
	virtual ~ActionNode();

	// Reads the node properties from an Xml archive.
	virtual void FromXml(FXmlNode * const node);

	// Gets the type of this node.
	FString const & GetType() const;

	// Performs some action.
	virtual void Execute(ASignalsBattleMode * const battle);

	// Update the action wrt time.
	virtual bool Update(ASignalsBattleMode * const battle, float dt);

	// Factory interface.
	typedef ActionNode * (*Ctor)();
	static Ctor RegisterCtor(FString const & type, Ctor ctor);

	// Called after node creation to modify the action appropriately.
	virtual void PostInitialize(Action * const action);

	// Is this a container node?
	const bool IsContainer;

	// Where to execute the node.
	Destination GetDestination() const;
	FString const & GetCustomTargetName() const;

	// Factory method for node creation.
	static ActionNode * Create(FString const & type);

private:
	FString _type;
	Destination _destination;
	FString _customTarget;
};

inline FString const & ActionNode::GetType() const
{
	return _type;
}

inline Destination ActionNode::GetDestination() const
{
	return _destination;
}

inline FString const & ActionNode::GetCustomTargetName() const
{
	return _customTarget;
}