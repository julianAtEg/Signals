// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "SignalsInstance.h"
#include "HumanPlayerStats.h"
#include "Action.h"
#include "Item.h"

static UHumanPlayerStats * loadStats(FString const & folder, FString const & name);

USignalsInstance::USignalsInstance(FObjectInitializer const & init)
	: Super(init)
	, BattleInfo()
	, Ergs(0)
{
}

void USignalsInstance::Init()
{
	UE_LOG(SignalsLog, Warning, TEXT("Manual Init()"));

	Super::Init();

	Action::Initialize();
	Item::Initialize();

	BattleInfo.OurCombatants.Add(TEXT("Alice"));
	BattleInfo.OurCombatants.Add(TEXT("Brandon"));
	BattleInfo.TheirCombatants.Add(TEXT("Maw"));

	auto aliceStats = loadStats(TEXT("Data"), TEXT("Alice"));
	_stats.Add(TEXT("Alice"), aliceStats);

	auto brandonStats = loadStats(TEXT("Data"), TEXT("Brandon"));
	_stats.Add(TEXT("Brandon"), brandonStats);

	Ergs = 100;
}

void USignalsInstance::Shutdown()
{
	Item::Terminate();
	Action::Terminate();
	for (auto & kv : _stats)
	{
		kv.Value->RemoveFromRoot();
		delete kv.Value;
	}

	Super::Shutdown();
}

UHumanPlayerStats * USignalsInstance::GetHumanPlayerStats(FString player)
{
	check(_stats.Contains(player));
	return(_stats[player]);
}

//-----------------------------------------------------------------------------

static UHumanPlayerStats * loadStats(FString const & folder, FString const & name)
{
	auto stats = NewObject<UHumanPlayerStats>();
	stats->AddToRoot();
	stats->LoadStaticData(name);
	auto statsFile = name + ".xml";
	auto gameFolder = FPaths::GameContentDir();
	auto dataPath = FPaths::Combine(*gameFolder, *folder, *statsFile);
	stats->Load(dataPath);
	stats->SetInitialValues();
	return stats;
}
