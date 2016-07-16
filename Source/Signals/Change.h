#pragma once

class UPlayerStats;

struct Change
{
	Change() = default;
	virtual ~Change() = default;

	virtual void Apply(UPlayerStats * player) = 0;
	virtual void Remove(UPlayerStats * player) = 0;
};
