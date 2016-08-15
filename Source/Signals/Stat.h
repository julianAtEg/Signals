#pragma once

#include "StatType.h"

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

//-----------------------------------------------------------------------------

class Stat
{
public:
	// Construct a stat to default value 0.
	Stat();

	// Construct a stat to a specific base value.
	Stat(int baseValue, int minValue=1, int maxValue=100);

	// Dtor.
	~Stat();

	// Gets the current stat value.
	int GetValue() const;

	// Get / set the current base value.
	int GetBaseValue() const;
	void SetBaseValue(int baseValue);
	
	// Add / remove modifiers.
	void AddModifier(StatModifier * mod);
	void RemoveModifier(StatModifier * mod);
	void ClearModifiers();

	// Set the value range.
	void SetMin(int min);
	void SetMax(int max);

	// Convenient assignment / conversion operators.
	Stat & operator=(int value);
	explicit operator int() const;

private:
	int _baseValue;
	int _min;
	int _max;
	mutable int _valueCache;
	mutable bool _dirty;
	TArray<StatModifier *> _modifiers;
};

inline int Stat::GetBaseValue() const
{
	return _baseValue;
}

inline Stat::operator int() const
{
	return GetValue();
}

inline Stat & Stat::operator=(int value)
{
	SetBaseValue(value);
	return *this;
}