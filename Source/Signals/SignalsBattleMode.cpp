// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "SignalsBattleMode.h"
#include "SignalsInstance.h"
#include "AssetHelper.h"
#include "HumanPlayerStats.h"
#include "CachedRandom.h"
#include "SignalsAction.h"

//-----------------------------------------------------------------------------

// Random number generator with look-ahead.
static CachedRandom s_rng( 1134771U ); // FDateTime::GetTicks() );

//-----------------------------------------------------------------------------

static bool selectActionAI(Combatant * combatant);

//-----------------------------------------------------------------------------

bool ASignalsBattleMode::CanSelectCommands() const
{
	return _commandsEnabled;
}

TArray<ACharacter *> & ASignalsBattleMode::GetHumanPlayers()
{
	return _players;
}

void ASignalsBattleMode::BeginPlay()
{
	Super::BeginPlay();

	auto world = GetWorld();
	if (world == nullptr)
	{
		UE_LOG(SignalsLog, Error, TEXT("world is null"));
		return;
	}

	// Find all the player starts in the scene.
	TSubclassOf<AActor> targetClass = APlayerStart::StaticClass();
	TArray<AActor *> playerStarts;
	UGameplayStatics::GetAllActorsOfClass(world, targetClass, playerStarts);
	UE_LOG(SignalsLog, Log, TEXT("Found %d player starts"), playerStarts.Num());
	for (int i = 0; i < playerStarts.Num(); ++i)
	{
		auto start = Cast<APlayerStart>(playerStarts[i]);
		auto name = start->GetName();
		int index = name[name.Len() - 1] - '0';
		if (name.StartsWith("Character"))
		{
			_playerStarts[index] = start;
		}
		else
		{
			_enemyStarts[index] = start;
		}
	}

	// Find all the cameras in the scene.
	TSubclassOf<AActor> cameraClass = ACameraActor::StaticClass();
	TArray<AActor *> cameras;
	UGameplayStatics::GetAllActorsOfClass(world, cameraClass, cameras);
	for (auto actor : cameras)
	{
		if (actor->GetName().StartsWith("Battle"))
		{
			_cameras.Add(Cast<ACameraActor>(actor));
		}
	}
	UE_LOG(SignalsLog, Log, TEXT("Found %d cameras"), _cameras.Num());

	// Get the battle setup from the game instance.
	auto instance = Cast<USignalsInstance>(world->GetGameInstance());
	if (instance == nullptr)
	{
		UE_LOG(SignalsLog, Error, TEXT("instance is null"));
		return;
	}

	// Spawn goodies and baddies.
	auto & info = instance->BattleInfo;
	initCombatants(world, _playerStarts, info.OurCombatants,  true);
	initCombatants(world, _enemyStarts, info.TheirCombatants, false);

	// Set up camera switching.
	_cameraSwitchTimer = 0.0f;
	if (_cameras.Num() > 0)
	{
		SwitchToCamera(0);
	}

	// Initialize the play schedule.
	for (int i = 0; i < _combatants.Num(); ++i)
	{
		auto & comb = _combatants[i];
		auto * stats = comb.Stats;
		auto speed = (int)FMath::Clamp((int)(stats->Speed + stats->Fortune*s_rng.HalfGaussian01()), 1, 100);
		comb.TurnDelay = 100.0f / speed;
		_scheduler.Add(i, comb.TurnDelay);
	}

	nextTurn();
}

void ASignalsBattleMode::SwitchToCamera(int camera)
{
	auto world = GetWorld();
	auto controller = UGameplayStatics::GetPlayerController( world, 0 );
	FViewTargetTransitionParams params;
	controller->SetViewTarget(_cameras[camera], params);
	_cameraIndex = camera;
}

void ASignalsBattleMode::InitializeInput(UInputComponent * input)
{

}

void ASignalsBattleMode::Tick(float dt)
{
	auto world = GetWorld();
	if (world != nullptr)
	{
		auto instance = Cast<USignalsInstance>(world->GetGameInstance());
		if (instance != nullptr)
		{
			if (_cameras.Num() > 1)
			{
				// Temporary camera view switching code.
				_cameraSwitchTimer += dt;
				if (_cameraSwitchTimer >= 5.0f)
				{
					int nextCamera = (_cameraIndex + 1) % _cameras.Num();
					SwitchToCamera(nextCamera);
					_cameraSwitchTimer -= 5.0f;
				}
			}

			// Main player loop
			auto currentPlayer = &_combatants[_currentPlayerIndex];
			if (updateCombatant(world, currentPlayer, dt))
			{
				nextTurn();
			}
		}
	}
	
}

bool ASignalsBattleMode::IsActiveCombatantHuman() const
{
	return _combatants[_currentPlayerIndex].IsHuman;
}

FString ASignalsBattleMode::GetHumanPlayer(int index) const
{
	UE_LOG(SignalsLog, Log, TEXT("Getting human player %d"), index);
	return _players[index]->GetName();
}

FString ASignalsBattleMode::GetActiveCombatant() const
{
	return _combatants[_currentPlayerIndex].Avatar->GetName();
}

void ASignalsBattleMode::OnTurnBeginning_Implementation(ACharacter * character, bool isHuman)
{

}

void ASignalsBattleMode::OnActionComplete()
{
	auto combatant = &_combatants[_currentPlayerIndex];
	combatant->State = ActionState::Complete;
}

void ASignalsBattleMode::nextTurn()
{
	_currentPlayerIndex = _scheduler.Next();
	auto player = &_combatants[_currentPlayerIndex];
	_commandsEnabled = false;
	OnTurnBeginning(player->Avatar, player->IsHuman);
}

void ASignalsBattleMode::SetCurrentCombatantAction(ActionInstance * action)
{
	auto combatant = &_combatants[_currentPlayerIndex];
	if (combatant->IsHuman)
	{
		_commandsEnabled = false;
	}

	combatant->Activity = action;
	combatant->TurnCounter = action->GetWarmupTurns();
	combatant->State = ActionState::Start;
}

bool ASignalsBattleMode::updateCombatant(UWorld * world, Combatant * combatant,float dt)
{
	bool advance = false;
	switch (combatant->State)
	{
		case ActionState::Idle:
			if (combatant->IsHuman)
			{
				// Wait for input.
				UE_LOG(SignalsLog, Log, TEXT("Waiting for input"));
				combatant->State = ActionState::Waiting;
				_commandsEnabled = true;
			}
			else
			{
				// Let the AI choose a command.
				if (selectActionAI(combatant))
				{
					combatant->State = ActionState::Start;
				}
				else
				{
					// Oops, the AI failed, next please.
					UE_LOG(SignalsLog,Warning,TEXT("Combatant %s skipped action selection"), *combatant->Avatar->GetName())
					advance = true;
					break;
				}
			}
			break;

		case ActionState::Waiting:
			// Nothing to do but wait till user selects a command.
			break;

		case ActionState::Start:
			combatant->State = ActionState::Warmup;
			combatant->Activity->RunWarmup(world, combatant);
			break;

		case ActionState::Warmup:
			if (combatant->TurnCounter == 0)
			{
				// Warmup has finished, time to run the action.
				checkf(combatant->Activity != nullptr, TEXT("Null action!"));
				combatant->State = ActionState::Running;
				combatant->Activity->RunAction(world, combatant);
			}
			else
			{
				// Still waiting so skip to the next player.
				--combatant->TurnCounter;
				advance = true;
			}
			break;

		case ActionState::Running:
			// Waiting for the action to complete.
			break;

		case ActionState::Complete:
			UE_LOG(SignalsLog, Log, TEXT("% turn complete"), *combatant->Avatar->GetName());
			// Run the (instantaneous) payload.
			combatant->Activity->RunPayload(world, combatant);
			delete combatant->Activity;
			combatant->Activity = nullptr;
			combatant->State = ActionState::Idle;
			advance = true;
			break;

	}

	return advance;
}

//-----------------------------------------------------------------------------

void ASignalsBattleMode::initCombatants(UWorld * world, APlayerStart * starts[], TArray<FString> const & combatants, bool human)
{
	switch (combatants.Num())
	{
	case 0:
		UE_LOG(SignalsLog, Error, TEXT("No combatants in battle info"));
		return;

	case 1:
		initCombatant( world, starts[1], combatants[0], human);
		break;

	case 2:
		initCombatant(world, starts[0], combatants[0], human);
		initCombatant(world, starts[2], combatants[1], human);
		break;

	case 3:
		initCombatant(world, starts[0], combatants[0], human);
		initCombatant(world, starts[1], combatants[1], human);
		initCombatant(world, starts[2], combatants[2], human);
		break;

	default:
		UE_LOG(SignalsLog, Error, TEXT("Too many combatants (%d)"), combatants.Num());
		break;
	}

}

void ASignalsBattleMode::initCombatant( UWorld * world, APlayerStart * start, FString actorId, bool human )
{
	UE_LOG(SignalsLog, Log, TEXT("Creating combatant %s"), *actorId);

	auto pos = start->GetActorLocation();
	auto rot = start->GetActorRotation();
	auto path = FString::Printf(TEXT("Blueprint'/Game/Blueprints/%sBP.%sBP'"), *actorId, *actorId);
	auto actor = AssetHelper::SpawnActorFromNamedBlueprint(world, pos, rot, path, actorId);
	auto character = Cast<ACharacter>(actor);
	if (character == nullptr)
	{
		UE_LOG(SignalsLog, Error, TEXT("Not a character"));
	}

	UPlayerStats * stats;
	if (human)
	{
		_players.Add(character);
		auto instance = Cast<USignalsInstance>(GetGameInstance());
		stats = instance->GetHumanPlayerStats(actorId);
	}
	else
	{
		_enemies.Add(character);
		// TODO: get enemy data.
		stats = NewObject<UPlayerStats>();
	}

	Combatant comb(human, character, stats);
	_combatants.Add(comb);
}

//-----------------------------------------------------------------------------

static bool selectActionAI(Combatant * combatant)
{
	return false;
}
