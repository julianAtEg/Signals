#pragma once

#include "Stat.h"

class StatModifier
{
public:
	virtual ~StatModifier() = default;
	virtual int Apply(int statValue) = 0;

	const EStatClass Stat;

protected:
	StatModifier(EStatClass stat)
		: Stat(stat)
	{

	}
};

class AddStatModifier : public StatModifier
{
public:
	AddStatModifier(EStatClass stat, int delta)
		: StatModifier(stat)
		, _delta(delta)
	{
	}

	int Apply(int value) override
	{
		return(value + _delta);
	}

private:
	int _delta;
};

class PercentageStatModifier : public StatModifier
{
public:
	PercentageStatModifier(EStatClass stat, int percent)
		: StatModifier(stat)
		, _percent(percent)
	{
	}

	int Apply(int value) override
	{
		int delta = (_percent*value) / 100;
		return value + delta;
	}

private:
	int _percent;
};
