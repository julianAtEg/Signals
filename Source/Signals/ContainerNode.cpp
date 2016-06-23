#include "Signals.h"
#include "ContainerNode.h"

ContainerNode::ContainerNode()
: ActionNode("container", true)
{

}

ContainerNode::ContainerNode(FString const & type)
: ActionNode(type, true)
{

}

ContainerNode::~ContainerNode()
{
	for (auto & child : _children)
	{
		delete child;
	}
}

void ContainerNode::FromXml(FXmlNode * const node)
{
	for (auto child = node->GetFirstChildNode(); child != nullptr; child = child->GetNextNode())
	{
		auto type = child->GetTag();
		auto childAction = ActionNode::Create(type);
		_children.Add(childAction);
		childAction->FromXml(const_cast<FXmlNode * const>(child));
	}
}

void ContainerNode::PostInitialize(Action * const action)
{
	for (auto & child : _children)
	{
		child->PostInitialize(action);
	}
}

ActionNode * ContainerNode::FindChildOfType(FString const & type)
{
	// SHALLOW search!
	for (auto & child : _children)
	{
		if (child->GetType() == type)
		{
			return child;
		}
	}

	return nullptr;
}

void ContainerNode::OnEnter(ASignalsBattleMode * const battle)
{

}

void ContainerNode::OnLeave(ASignalsBattleMode * const battle)
{

}

void ContainerNode::LoadResources(ASignalsBattleMode * const battle)
{
	for (auto & child : _children)
	{
		child->LoadResources(battle);
	}
}

void ContainerNode::UnloadResources(ASignalsBattleMode * const battle)
{
	for (auto & child : _children)
	{
		child->UnloadResources(battle);
	}
}