#pragma once

struct Change;
class UPlayerStats;

class ItemActionNode
{
public:
	virtual ~ItemActionNode() = default;
	virtual void Apply(UPlayerStats * stats) = 0;
	virtual void Remove(UPlayerStats * stats) = 0;

protected:
	ItemActionNode() = default;
};