#pragma once

#include "ActionNode.h"
#include "StatType.h"

/**
 * Node for complex stat operations.
 */
class StatNode : public ActionNode
{
public:
	void FromXml(FXmlNode * const node) override;
	void PostInitialize(Action * const action) override;

	EStatClass GetType() const;
	int GetBase() const;
	int GetLevelScale() const;
	int GetMin() const;
	int GetMax() const;
	bool IsFixed() const;

protected:
	StatNode(FString const & type);

private:
	EStatClass _type;
	int _base;
	int _levelScale;
	int _min;
	int _max;
	bool _isFixed;
};

inline bool StatNode::IsFixed() const
{
	return _isFixed;
}

inline EStatClass StatNode::GetType() const
{
	return _type;
}

inline int StatNode::GetBase() const
{
	return _base;
}

inline int StatNode::GetLevelScale() const
{
	return _levelScale;
}

inline int StatNode::GetMin() const
{
	return _min;
}

inline int StatNode::GetMax() const
{
	return _max;
}
