#include "Signals.h"
#include "Stat.h"

Stat::Stat()
: Stat(1,1,100)
{

}

Stat::Stat( int value, int min, int max )
: _baseValue(value)
, _min(min)
, _max(max)
, _valueCache(0)
, _dirty(true)
, _modifiers()
{

}

Stat::~Stat()
{
	ClearModifiers();
}

int Stat::GetValue() const
{
	if (_dirty)
	{
		int value = _baseValue;
		for (auto mod : _modifiers)
		{
			value = mod->Apply(value);
		}

		_valueCache = FMath::Clamp(value, _min, _max);
		_dirty = false;
	}

	return _valueCache;
}

void Stat::SetBaseValue(int value)
{
	_baseValue = value;
	_dirty = true;
}

void Stat::AddModifier(StatModifier * mod)
{
	_modifiers.Add(mod);
	_dirty = true;
}

void Stat::RemoveModifier(StatModifier * mod)
{
	_modifiers.Remove(mod);
	delete mod;
	_dirty = true;
}

void Stat::ClearModifiers()
{
	for (auto mod : _modifiers)
	{
		delete mod;
	}
	_modifiers.Empty();
	_dirty = true;
}

void Stat::SetMin(int min)
{
	if (min != _min)
	{
		_min = min;
		_dirty = true;
	}
}

void Stat::SetMax(int max)
{
	if (max != _max)
	{
		_max = max;
		_dirty = true;
	}
}