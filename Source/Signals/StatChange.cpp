#include "Signals.h"
#include "StatChange.h"
#include "PlayerStats.h"

StatChange::StatChange(EStatClass stat, int delta)
: Change()
, _theStat(stat)
, _delta(delta)
{

}

void StatChange::Apply(UPlayerStats * stats)
{
	stats->ApplyStatChange(_theStat, _delta, false);
}

void StatChange::Remove(UPlayerStats * stats)
{
	stats->ApplyStatChange(_theStat, -_delta, false);
}