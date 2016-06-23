#pragma once

#include "ActionNode.h"

// Node type that contains other nodes.
class ContainerNode : public ActionNode
{
public:
	ContainerNode();
	ContainerNode(FString const & type);
	~ContainerNode();

	void FromXml(FXmlNode * const node) override;
	void PostInitialize(Action * const action) override;

	TArray<ActionNode *> & Children();
	TArray<ActionNode *> const & Children() const;
	int GetChildCount() const;
	ActionNode * GetChild(int index);
	ActionNode const * GetChild(int index) const;

	// Finds a child node of the given type (shallow).
	ActionNode * FindChildOfType(FString const & name);

	virtual void OnEnter(ASignalsBattleMode * battle);
	virtual void OnLeave(ASignalsBattleMode * battle);

	void LoadResources(ASignalsBattleMode * const battle) override;
	void UnloadResources(ASignalsBattleMode * const battle) override;

private:
	TArray<ActionNode *> _children;
};

inline int ContainerNode::GetChildCount() const
{
	return _children.Num();
}

inline ActionNode * ContainerNode::GetChild(int index)
{
	return _children[index];
}

inline ActionNode const * ContainerNode::GetChild(int index) const
{
	return _children[index];
}

inline TArray<ActionNode *> & ContainerNode::Children()
{
	return _children;
}

inline TArray<ActionNode *> const & ContainerNode::Children() const
{
	return _children;
}
