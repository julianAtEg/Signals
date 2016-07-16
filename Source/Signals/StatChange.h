#pragma once

#include "Change.h"
#include "StatType.h"

struct StatChange : public Change
{
	StatChange(EStatClass stat, int delta);

	void Apply(UPlayerStats * stats) override;
	void Remove(UPlayerStats * stats) override;

private:
	EStatClass const _theStat;
	int const _delta;
};
