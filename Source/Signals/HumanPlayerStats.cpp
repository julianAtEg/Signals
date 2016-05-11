// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "HumanPlayerStats.h"

UHumanPlayerStats::UHumanPlayerStats(FObjectInitializer const & init)
: Super(init)
, _displayHitPoints( 0 )
{

}

void UHumanPlayerStats::Update(float dt)
{
	if (_displayHitPoints < HitPoints)
	{
		++_displayHitPoints;
	}
	else if (_displayHitPoints > HitPoints)
	{
		--_displayHitPoints;
	}
}

int UHumanPlayerStats::GetDisplayHitPoints() const
{
	return _displayHitPoints;
}
