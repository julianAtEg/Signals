// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "SignalsInstance.h"
#include "HumanPlayerStats.h"

USignalsInstance::USignalsInstance(FObjectInitializer const & init)
	: Super(init)
{
}

void USignalsInstance::Init()
{
	UE_LOG(SignalsLog, Warning, TEXT("Manual Init()"));

	BattleInfo.OurCombatants.Add(TEXT("Alice"));
	BattleInfo.OurCombatants.Add(TEXT("Brandon"));
	BattleInfo.TheirCombatants.Add(TEXT("Maw"));

	auto aliceStats = NewObject<UHumanPlayerStats>();
	//FBattleStats aliceStats;
	aliceStats->HitPoints = aliceStats->MaxHitPoints = 50;
	//aliceStats.Energy = aliceStats.MaxEnergy = 100;
	_stats.Add(TEXT("Alice"), aliceStats);

	auto brandonStats = NewObject<UHumanPlayerStats>();
	brandonStats->HitPoints = brandonStats->MaxHitPoints = 60;
	//////brandonStats.Energy = brandonStats.MaxEnergy = 120;
	_stats.Add(TEXT("Brandon"), brandonStats);
}

UHumanPlayerStats * USignalsInstance::GetHumanPlayerStats(FString player)
{
	return(_stats[player]);
}

void USignalsInstance::UpdateStats(float dt)
{
	for (auto & entry : _stats)
	{
		auto & stats = entry.Value;
		stats->Update(dt);
	}
}