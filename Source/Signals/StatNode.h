#pragma once

#include "ActionNode.h"
#include "StatType.h"

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

protected:
	StatNode(FString const & type);

private:
	EStatClass _type;
	int _base;
	int _levelScale;
	int _min;
	int _max;
};

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
