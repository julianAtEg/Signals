// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "SignalsBattleMode.h"
#include "SignalsInstance.h"
#include "AssetHelper.h"
#include "HumanPlayerStats.h"
#include "CachedRandom.h"
#include "Action.h"
#include "ActionInstance.h"
#include "UIEvent.h"
#include "ActionMenuItem.h"

//-----------------------------------------------------------------------------

// Random number generator with look-ahead.
static CachedRandom s_rng( 1134771U ); // FDateTime::GetTicks() );

//-----------------------------------------------------------------------------

static bool selectActionAI(Combatant * combatant);
static bool compareItems(FActionMenuItem const & a1, FActionMenuItem const & a2);
static void getEnemies(TArray<Combatant *> & enemies, TArray<Combatant> & all);
static void getFriends(TArray<Combatant *> & friends, TArray<Combatant> & all, Combatant * me);

//-----------------------------------------------------------------------------

FString ASignalsBattleMode::GetInfoText() const
{
	return _infoText;
}

int ASignalsBattleMode::GetInfoIcon() const
{
	return _infoIcon;
}

void ASignalsBattleMode::SelectTarget()
{
	Combatant * target = _targets[_currentTarget];
	UE_LOG(SignalsLog, Log, TEXT("Target '%s' selected"), *target->Avatar->GetName());

	// Replace the potential targets with the actual one.
	_targets.Empty();
	_targets.Add(target);

	HideTargetMarker();
	check(_selectedAction != nullptr);
	auto actionInst = new ActionInstance(_selectedAction);
	SetCurrentCombatantAction(actionInst);
	_selectedAction = nullptr;
}

void ASignalsBattleMode::NextTarget()
{
	++_currentTarget;
	if (_currentTarget == _targets.Num())
	{
		_currentTarget = 0;
	}

	ShowTargetMarker(_targets[_currentTarget]->Avatar);	
}

void ASignalsBattleMode::PreviousTarget()
{
	--_currentTarget;
	if (_currentTarget < 0)
	{
		_currentTarget = _targets.Num() - 1;
	}
	ShowTargetMarker(_targets[_currentTarget]->Avatar);
}

void ASignalsBattleMode::CancelTarget()
{
	_menuState = MenuState::SelectAction;
	_selectedAction = nullptr;
	_targets.Empty();
	HideTargetMarker();
	ShowActiveMarker();
}

MenuState ASignalsBattleMode::GetMenuState() const
{
	return _menuState;
}

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
		auto speed = (int)FMath::Clamp((int)(stats->Speed * (1+s_rng.HalfGaussian01())), 1, 100);
		comb.TurnDelay = 100.0f / speed;
		_scheduler.Add(i, comb.TurnDelay);
	}

	HideTargetMarker();
	_menuState = MenuState::SelectAction;
	_selectedAction = nullptr;
	_firstTurn = true;
	_infoIcon = -1;
	_infoText = TEXT("");
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
	UE_LOG(SignalsLog, Log, TEXT("Setting BATTLE controls"));
	input->BindAction("MenuRight", IE_Pressed, this, &ASignalsBattleMode::OnMenuRight);
	input->BindAction("MenuLeft", IE_Pressed, this, &ASignalsBattleMode::OnMenuLeft);
	input->BindAction("MenuSelect", IE_Pressed, this, &ASignalsBattleMode::OnMenuSelect);
	input->BindAction("MenuBack", IE_Pressed, this, &ASignalsBattleMode::OnMenuBack);
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
	return _players[index]->GetName();
}

FString ASignalsBattleMode::GetActiveCombatant() const
{
	return _combatants[_currentPlayerIndex].Avatar->GetName();
}

void ASignalsBattleMode::RunActionPayload()
{
	auto combatant = &_combatants[_currentPlayerIndex];
	combatant->Activity->RunPayload(this);
}

void ASignalsBattleMode::OnActionComplete()
{
	auto combatant = &_combatants[_currentPlayerIndex];
	combatant->State = ActionState::Complete;
}

void ASignalsBattleMode::nextTurn()
{
	_menuItems.Empty();
	_selectedItem = nullptr;
	_currentPlayerIndex = _scheduler.Next();
	auto player = &_combatants[_currentPlayerIndex];
	_infoIcon = -1;
	if (player->IsHuman)
	{
		findAvailableActions(player);
		_menuState = MenuState::SelectAction;
		_infoText = FString::Printf(TEXT("%s: select an action"), *player->Avatar->GetName());
	}
	else
	{
		_infoText = FString::Printf(TEXT("%s..."), *player->Avatar->GetName());
	}
	EnablePlayerInput(false,_firstTurn);
	_firstTurn = false;
	OnTurnBeginning(player->Avatar, player->IsHuman);
}

void ASignalsBattleMode::findAvailableActions(Combatant * const combatant)
{
	TMap<FString, int> categoryIDs;
	auto instance = Cast<USignalsInstance>(GetWorld()->GetGameInstance());
	auto stats = instance->GetHumanPlayerStats(combatant->Avatar->GetName());
	auto actionNames = stats->GetAvailableActionNames();
	int ID = 0;
	for (auto & name : actionNames)
	{
		auto action = Action::FindAction(name);
		if (action != nullptr)
		{
			FActionMenuItem ami;
			ami.IconIndex = action->GetMenuIcon();
			auto cat = action->GetCategory();
			if (cat.IsEmpty())
			{
				// Bar menu item.
				ami.Text = action->GetName();
				ami.Description = action->GetDescription();
				ami.IsLeaf = true;
				ami.ID = ID++;
				ami.Level = 0;
				ami.ParentID = -1;
				_menuItems.Add(ami);
			}
			else
			{
				// There's a category, add a menu item if we've not seen it before.
				if (!categoryIDs.Contains(cat))
				{
					FActionMenuItem catAmi;
					catAmi.Text = cat + TEXT(">");
					catAmi.IsLeaf = false;
					catAmi.ID = ID++;
					catAmi.Level = 0;
					catAmi.ParentID = -1;
					_menuItems.Add(catAmi);
					categoryIDs.Add(cat, catAmi.ID);
				}

				// Add an item for the action as well.
				ami.Text = action->GetName();
				ami.Description = action->GetDescription();
				ami.IsLeaf = true;
				ami.ID = ID++;
				ami.Level = 1;
				ami.ParentID = categoryIDs[cat];
				_menuItems.Add(ami);
			}
		}
		else
		{
			UE_LOG(SignalsLog, Error, TEXT("Could not find action '%s'"), *name);
		}
	}
}

void ASignalsBattleMode::SetCurrentCombatantAction(ActionInstance * action)
{
	auto combatant = &_combatants[_currentPlayerIndex];
	if (combatant->IsHuman)
	{
		EnablePlayerInput(false);
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
				EnablePlayerInput(true);
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
			UE_LOG(SignalsLog, Log, TEXT("Player %s starting"), *combatant->Avatar->GetName());
			combatant->State = ActionState::Warmup;
			combatant->Activity->RunWarmup(this);
			break;

		case ActionState::Warmup:
			if (combatant->TurnCounter == 0)
			{
				// Warmup has finished, time to run the action.
				checkf(combatant->Activity != nullptr, TEXT("Null action!"));
				UE_LOG(SignalsLog, Log, TEXT("Player %s running action %s"), *combatant->Avatar->GetName(), *combatant->Activity->GetName());
				combatant->State = ActionState::Running;
				combatant->Activity->RunActivity(this);
			}
			else
			{
				// Still waiting so skip to the next player.
				--combatant->TurnCounter;
				advance = true;
				UE_LOG(SignalsLog, Log, TEXT("Player %s still warming up"), *combatant->Avatar->GetName());
			}
			break;

		case ActionState::Running:
			// Waiting for the action to complete.
			combatant->Activity->Update(this, dt);
			break;

		case ActionState::Complete:
			UE_LOG(SignalsLog, Log, TEXT("%s turn complete"), *combatant->Avatar->GetName());
			delete combatant->Activity;
			combatant->Activity = nullptr;
			combatant->State = ActionState::Idle;
			advance = true;
			break;

	}

	return advance;
}

TArray<FActionMenuItem> ASignalsBattleMode::GetAvailableActions( int level ) const
{
	TArray<FActionMenuItem> actions;

	for (auto & ami : _menuItems)
	{
		bool add = false;
		if (level == 0)
		{
			add = (ami.Level == 0);
		}
		else
		{
			// Level is 1, so category must match selected.
			add = ((ami.Level == 1) && (ami.ParentID == _selectedItem->ID));
		}
		if (add)
		{ 
			actions.Add(ami);
		}
	}

	actions.Sort(compareItems);

	return actions;
}

void ASignalsBattleMode::EnablePlayerInput(bool enable,bool force)
{
	if ((_commandsEnabled != enable) || force)
	{
		_commandsEnabled = enable;
		UpdateUI();
	}
}

void ASignalsBattleMode::HandleMenuSelect(int itemID)
{
	_selectedItem = _menuItems.FindByPredicate([itemID](FActionMenuItem const & item)
	{
		return(item.ID == itemID);
	});
	if (_selectedItem->IsLeaf)
	{
		// TODO: This is an action, so let's invoke it.
		auto actionName = _selectedItem->Text;
		UE_LOG(SignalsLog, Log, TEXT("Action %s selected"), *actionName);
		auto action = Action::FindAction(actionName);
		checkf(action != nullptr, TEXT("No such action: %s"), *action);
		_targets.Empty();
		auto player = &_combatants[_currentPlayerIndex];
		if (action->AffectsMultipleTargets())
		{
			// TODO: consider manual selection of either side rather than auto-targetting.
			if (action->IsOffensive())
			{
				getEnemies(_targets, _combatants);
			}
			else
			{
				getFriends(_targets, _combatants, player);
			}
			auto actionInst = new ActionInstance(action);
			SetCurrentCombatantAction(actionInst);
		}
		else
		{
			_selectedAction = action;
			_currentTarget = 0;
			_menuState = MenuState::SelectTarget;
			// Offensive actions should select the enemies first; otherwise Our Side.
			TArray<Combatant *> friends;
			TArray<Combatant *> enemies;
			if (action->IsOffensive())
			{
				getEnemies(_targets, _combatants);
				getFriends(_targets, _combatants, player);
			}
			else
			{
				getFriends(_targets, _combatants, player);
				getEnemies(_targets, _combatants);
			}
			HideActiveMarker();
			ShowTargetMarker(_targets[0]->Avatar);
			_infoIcon = action->GetMenuIcon();
			_infoText = FString::Printf(TEXT("Target: %s"), *_targets[0]->Avatar->GetName());
			UpdateUI();
		}

	}
}

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
		stats->AddToRoot();
	}

	Combatant comb(human, character, stats);
	_combatants.Add(comb);
}

//-----------------------------------------------------------------------------

static bool selectActionAI(Combatant * combatant)
{
	return false;
}

static bool compareItems(FActionMenuItem const & a1, FActionMenuItem const & a2)
{
	if (a1.IsLeaf == a2.IsLeaf)
	{
		return(a1.Text < a2.Text);
	}
	else if (a1.IsLeaf)
	{
		return false;
	}
	else
	{
		return true;
	}
}

static void getEnemies(TArray<Combatant *> & enemies, TArray<Combatant> & all)
{
	for(auto & combatant : all)
	{
		// ToDO: only consider living ones.
		if (!combatant.IsHuman)
		{
			enemies.Add(&combatant);
		}
	}
}

static void getFriends(TArray<Combatant *> & friends, TArray<Combatant> & all, Combatant * me)
{
	// Make sure the instigator is first in the list.
	friends.Add(me);
	for (auto & combatant : all)
	{
		// ToDO: only consider living ones.
		if (combatant.IsHuman && (&combatant != me))
		{
			friends.Add(&combatant);
		}
	}
}

