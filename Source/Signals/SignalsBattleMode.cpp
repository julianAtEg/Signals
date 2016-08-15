// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "SignalsBattleMode.h"
#include "SignalsInstance.h"
#include "AssetHelper.h"
#include "HumanPlayerStats.h"
#include "NpcPlayerStats.h"
#include "CachedRandom.h"
#include "Action.h"
#include "ActionInstance.h"
#include "UIEvent.h"
#include "ActionMenuItem.h"
#include "Combat.h"
#include "ActionClass.h"
#include "Strategy.h"
#include "ResourceManager.h"
#include "Item.h"
#include "BattleTask.h"

//-----------------------------------------------------------------------------

// Number of items in the turn schedule.
static const int SCHEDULE_SIZE = 6;

// Random number generator with look-ahead.
static CachedRandom s_rng( 1134771U ); // FDateTime::GetTicks() );

//-----------------------------------------------------------------------------

static bool selectActionAI(ASignalsBattleMode * battle,Combatant * combatant);
static bool compareItems(FActionMenuItem const & a1, FActionMenuItem const & a2);
static void getEnemies(TArray<Combatant *> & enemies, TArray<Combatant> & all);
static void getFriends(TArray<Combatant *> & friends, TArray<Combatant> & all, Combatant * me);
static bool checkForWin(TArray<Combatant> const & players, bool human);
static void loadStaticData(UResourceManager * resMgr);

//-----------------------------------------------------------------------------

void ASignalsBattleMode::AddTask(BattleTask * task)
{
	check(task != nullptr); // Is valid.

	_tasks.Schedule(task);
}

TArray<int> ASignalsBattleMode::GetSchedule() const
{
	TArray<int> result;
	if (_currentPlayerIndex >= 0 && (_scheduler.Size() > 0))
	{
		result.Add(_combatants[_currentPlayerIndex].Stats->IconIndex);
		for (int i = 0; i < _scheduler.Size(); ++i)
		{
			auto index = _scheduler.Peek(i);
			auto icon = _combatants[index].Stats->IconIndex;
			result.Add(icon);
		}
	}
	return result;
}

void ASignalsBattleMode::ReschedulePlayer( Combatant * player )
{
	UE_LOG(SignalsLog, Log, TEXT("ASignalsBattleMode::ReschedulePlayer()"));

	int index = player-&_combatants[0];
	_scheduler.Cancel(index);
	player->TurnDelay = 0;
	while (_scheduler.Size() < SCHEDULE_SIZE)
	{
		scheduleTurn(index);
	}
	dumpSchedule();
}

TArray<Combatant *> ASignalsBattleMode::GetActiveHumans() const
{
	TArray<Combatant *> result;
	for (auto & player : _combatants)
	{
		if (player.IsAlive && player.IsHuman)
		{
			result.Add(const_cast<Combatant *>(&player));
		}
	}

	return result;
}

TArray<Combatant *> ASignalsBattleMode::GetActiveNPCs() const
{
	TArray<Combatant *> result;
	for (auto & player : _combatants)
	{
		if (player.IsAlive && !player.IsHuman)
		{
			result.Add(const_cast<Combatant *>(&player));
		}
	}

	return result;
}

Random * ASignalsBattleMode::GetRandom() const
{
	return &s_rng;
}

FString ASignalsBattleMode::GetInfoText() const
{
	return _infoText;
}

int ASignalsBattleMode::GetInfoIcon() const
{
	return _infoIcon;
}

bool ASignalsBattleMode::IsBoostActive() const
{
	return _boostGaugeActive;
}

float ASignalsBattleMode::GetBoostFraction() const
{
	return _boostTime / _boostMaxTime;
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

	refreshTargetInfo();
}

void ASignalsBattleMode::refreshTargetInfo()
{
	_infoText = FString::Printf(TEXT("Target: %s"), *_targets[_currentTarget]->Avatar->GetName());
	ShowTargetMarker(_targets[_currentTarget]->Avatar);
	UpdateUI();
}

void ASignalsBattleMode::PreviousTarget()
{
	--_currentTarget;
	if (_currentTarget < 0)
	{
		_currentTarget = _targets.Num() - 1;
	}
	refreshTargetInfo();
}

void ASignalsBattleMode::CancelTarget()
{
	_menuState = EMenuState::SelectAction;
	_selectedAction = nullptr;
	_targets.Empty();
	HideTargetMarker();
	ShowActiveMarker();
}

EMenuState ASignalsBattleMode::GetMenuState() const
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

EBattleState ASignalsBattleMode::GetBattleState() const
{
	return _state;
}

void ASignalsBattleMode::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	_tasks.Finish(this);
	if (_resMgr != nullptr)
	{
		if (_resMgr->IsValidLowLevel())
		{
			_resMgr->RemoveFromRoot();
			_resMgr->ConditionalBeginDestroy();
			_resMgr = nullptr;
		}
	}
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

	_resMgr = NewObject<UResourceManager>();
	_resMgr->AddToRoot();

	_state = EBattleState::BattleIdle;

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

	// Load resources, avoiding duplication.
	TSet<FString> actionNames;
	for (auto & combatant : _combatants)
	{
		auto stats = combatant.Stats;
		auto playerActions = stats->GetActions();
		actionNames.Append(playerActions);
	}
	for (auto actionName : actionNames)
	{
		auto action = Action::FindAction(actionName);
		action->LoadResources(this);
	}

	// Set up camera switching.
	_cameraSwitchTimer = 0.0f;
	if (_cameras.Num() > 0)
	{
		SwitchToCamera(0);
	}

	// Initialize the play schedule.
	for (int i = 0; i < _combatants.Num(); ++i)
	{
		_combatants[i].TurnDelay = 0;
	}
	for (int i = 0; _scheduler.Size() < SCHEDULE_SIZE; ++i )
	{
		int playerIndex = i % _combatants.Num();
		scheduleTurn(playerIndex);
	}
	dumpSchedule();

	HideTargetMarker();
	_menuState = EMenuState::SelectAction;
	_selectedAction = nullptr;
	_infoIcon = -1;
	_infoText = TEXT("");
	_showSchedule = true;
	_state = EBattleState::InProgress;
	nextTurn( true );
}

void ASignalsBattleMode::dumpSchedule()
{
	UE_LOG(SignalsLog, Log, TEXT("------------------"));
	for (int i = 0; i < SCHEDULE_SIZE; ++i)
	{
		int playerIndex = _scheduler.Peek(i);
		auto c = &_combatants[playerIndex];
		UE_LOG(SignalsLog, Log, TEXT("%d: %s"), i + 1, *c->Avatar->GetName());
	}
	UE_LOG(SignalsLog, Log, TEXT("------------------"));
}

void ASignalsBattleMode::scheduleTurn(int playerIndex)
{
	auto * comb = &_combatants[playerIndex];
	auto * stats = comb->Stats;
	auto playerSpeed = stats->GetStat(EStatClass::Speed);
	auto speed = (int)FMath::Clamp((int)(playerSpeed * (1 + s_rng.HalfGaussian01())), 1, 100);
	comb->TurnDelay += 100.0f / speed;
	_scheduler.Add(playerIndex, comb->TurnDelay);
}

void ASignalsBattleMode::CycleCameras()
{
	int cam = _cameraIndex+1;
	if (cam >= _cameras.Num())
	{
		cam = 0;
	}
	SwitchToCamera(cam);
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
	input->BindAction("MenuUp", IE_Pressed, this, &ASignalsBattleMode::OnMenuUp);
	input->BindAction("MenuDown", IE_Pressed, this, &ASignalsBattleMode::OnMenuDown);
	input->BindAction("CycleCameras", IE_Pressed, this, &ASignalsBattleMode::CycleCameras);
}

void ASignalsBattleMode::Tick(float dt)
{
	auto world = GetWorld();
	if (world != nullptr)
	{
		auto instance = Cast<USignalsInstance>(world->GetGameInstance());
		if (instance != nullptr)
		{
			//if (_cameras.Num() > 1)
			//{
			//	// Temporary camera view switching code.
			//	_cameraSwitchTimer += dt;
			//	if (_cameraSwitchTimer >= 5.0f)
			//	{
			//		int nextCamera = (_cameraIndex + 1) % _cameras.Num();
			//		SwitchToCamera(nextCamera);
			//		_cameraSwitchTimer -= 5.0f;
			//	}
			//}

			// Main player loop
			switch (_state)
			{
			case EBattleState::BattleIdle:
				break;

			case EBattleState::InProgress:
			{
				auto currentPlayer = &_combatants[_currentPlayerIndex];
				if (updateCombatant(world, currentPlayer, dt))
				{
					nextTurn(false);
				}
				break;
			}

			case EBattleState::WonByHumans:
				UE_LOG(SignalsLog, Log, TEXT("Humans win!"));
				// TODO
				break;

			case EBattleState::WonByOthers:
				UE_LOG(SignalsLog, Log, TEXT("Others win!"));
				// TODO
				break;
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

TArray<FInventoryEntry> ASignalsBattleMode::GetActiveCombatantItems(EItemProperty filter) const
{
	auto player = &_combatants[_currentPlayerIndex];
	check(player->IsHuman);
	auto stats = Cast<UHumanPlayerStats>(player->Stats);
	auto inventory = stats->GetInventory();
	return inventory.GetItems(filter);
}

void ASignalsBattleMode::RunActionPayload()
{
	auto combatant = &_combatants[_currentPlayerIndex];
	combatant->Activity->RunPayload(this);
}

void ASignalsBattleMode::OnActionComplete()
{	
	UE_LOG(SignalsLog, Log, TEXT("ASignalsBattleMode::OnActionComplete()"));

	auto combatant = &_combatants[_currentPlayerIndex];
	auto currentAction = combatant->Activity;
	currentAction->NotifyActionComplete(this);
}

void ASignalsBattleMode::applyDamageToPlayer(int playerIndex, int damage)
{
	Combatant * player = &_combatants[playerIndex];
	UE_LOG(SignalsLog, Warning, TEXT("Damaging %s"), *player->Avatar->GetName());
	FString text;
	FVector color(1, 1, 1);
	if (damage > 0)
	{
		text = FString::Printf(TEXT("-%dHP"), damage);
	}
	else
	{
		text = TEXT("0HP");
	}

	AddFloatingNotification(player->Avatar, text, color);

	auto stats = player->Stats;
	auto HP = stats->GetStat(EStatClass::HitPoints);
	HP -= damage;
	bool dead = false;
	if (HP <= 0)
	{
		HP = 0;
		dead = true;
	}
	stats->SetStat(EStatClass::HitPoints, HP);

	if (dead)
	{
		// Remove player from schedule.
		_scheduler.Cancel(playerIndex);

		player->IsAlive = false;
		PlayAnimation(player->Avatar, TEXT("Death"), nullptr, false);

		// Check for win / lose.
		if (player->IsHuman)
		{
			// Check all human players to see if they're alive.
			if (checkForWin(_combatants, false))
			{
				// Win!
				_state = EBattleState::WonByHumans;
			}
		}
		else
		{
			// Check all non-human players.
			if (checkForWin(_combatants, true))
			{
				// Lose!.
				_state = EBattleState::WonByOthers;
			}
		}
	}
}

void ASignalsBattleMode::ApplyDamage()
{
	for( int i = 0; i < _combatants.Num(); ++i )
	{
		auto & combatant = _combatants[i];
		if (combatant.TookDamage)
		{
			applyDamageToPlayer(i, combatant.HPDamageThisTurn);
		}
		else if (combatant.ActionMissed)
		{
			FVector color(1, 1, 1);
			AddFloatingNotification(combatant.Avatar, TEXT("Miss"), color);
		}

		combatant.HPDamageThisTurn = 0;
		combatant.ActionMissed = false;
		combatant.TookDamage = false;
	}
}

void ASignalsBattleMode::nextTurn(bool firstTurn)
{
	// Add the current player back to the schedule.
	if (!firstTurn)
	{
		scheduleTurn(_currentPlayerIndex);
	}

	// Reset the state, and the UI too.
	_menuItems.Empty();
	_targets.Empty();
	_actionArgs.Empty();
	_selectedItem = nullptr;
	_infoIcon = -1;
	_boostGaugeActive = false;
	_boostTime = 0.0f;
	_boostMaxTime = 1.0f;

	_tasks.RunPendingTasks(this);
	Combatant * player = nullptr;
	do
	{
		if (_scheduler.Size() == 0)
		{
			UE_LOG(SignalsLog, Warning, TEXT("Empty schedule!?"));
			_state = EBattleState::BattleIdle;
			return;
		}

		_currentPlayerIndex = _scheduler.Next();
		player = &_combatants[_currentPlayerIndex];
		checkf(player->IsAlive, TEXT("Dead player in the schedule?"));
		player->OnTurnBeginning();

		// If player is sick, knock off some hit points. They might die. Tragic. 
		// If so, move on to the next player.
		if (player->HasStatus(EPlayerStatus::Sick))
		{
			auto HP = player->Stats->GetStat(EStatClass::HitPoints);
			auto delta = FMath::Max(10, HP / 10);
			applyDamageToPlayer(_currentPlayerIndex, delta);
		}
	} 
	while (!player->IsAlive);

	// If player is paralyzed, miss a turn.
	if (player->HasStatus(EPlayerStatus::Paralyzed))
	{
		player->State = ActionState::SkipTurn;
		return;
	}

	if (player->IsHuman)
	{
		// Boost gauge.
		auto stats = Cast<UHumanPlayerStats>(player->Stats);
		_boostGaugeActive = stats->HasSkill(BattleSkill::BoostSkill);
		if (_boostGaugeActive)
		{
			_boostMaxTime = Combat::GetBoostTime(stats->Level);
			_boostTime = _boostMaxTime;
		}

		findAvailableActions(player);
		_menuState = EMenuState::SelectAction;
		_infoText = FString::Printf(TEXT("%s: select an action"), *player->Avatar->GetName());
	}
	else
	{
		_infoText = FString::Printf(TEXT("%s..."), *player->Avatar->GetName());
	}
	EnablePlayerInput(false, firstTurn);
	OnTurnBeginning(player->Avatar, player->IsHuman);
	RefreshScheduleUI(_showSchedule);

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
				ami.IsSelectable = true;
				ami.ID = ID++;
				ami.Level = 0;
				ami.ParentID = -1;
				ami.ItemType = ItemType::RunAction;
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
					ami.IsSelectable = true;
					catAmi.ID = ID++;
					catAmi.Level = 0;
					catAmi.ParentID = -1;
					_menuItems.Add(catAmi);
					ami.ItemType = ItemType::RunAction;
					categoryIDs.Add(cat, catAmi.ID);
				}

				// Add an item for the action as well.
				ami.Text = action->GetName();
				ami.Description = action->GetDescription();
				ami.IsSelectable = (action->GetCost() <= instance->Ergs);
				ami.IsLeaf = true;
				ami.ID = ID++;
				ami.Level = 1;
				ami.ParentID = categoryIDs[cat];
				ami.ItemType = ItemType::RunAction;
				_menuItems.Add(ami);
			}
		}
		else
		{
			UE_LOG(SignalsLog, Error, TEXT("Could not find action '%s'"), *name);
		}
	}

	// Always add an "item" item :-)
	FActionMenuItem itemItem;
	itemItem.Text = TEXT("Item");
	itemItem.Description = TEXT("Use an item from the inventory");
	itemItem.IsSelectable = true;
	itemItem.IsLeaf = true;
	itemItem.Level = 0;
	itemItem.ID = ID++;
	itemItem.ParentID = -1;
	itemItem.ItemType = ItemType::UseInventoryItem;
	_menuItems.Add(itemItem);
}

void ASignalsBattleMode::SetActionTargets(TArray<Combatant *> const & targets)
{
	_targets.Append(targets);
}

void ASignalsBattleMode::SetCurrentCombatantAction(ActionInstance * action)
{
	auto combatant = &_combatants[_currentPlayerIndex];
	if (combatant->IsHuman)
	{
		EnablePlayerInput(false);
		auto game = Cast<USignalsInstance>(GetWorld()->GetGameInstance());
		game->Ergs -= action->GetAction()->GetCost();
		if (game->Ergs < 0)
		{
			game->Ergs = 0;
		}
		UpdateUI();
	}
	else
	{
		auto stats = Cast<UNpcPlayerStats>(combatant->Stats);
		stats->Ergs -= action->GetAction()->GetCost();
		if (stats->Ergs < 0)
		{
			stats->Ergs = 0;
		}
	}

	combatant->Activity = action;
	combatant->TurnCounter = action->GetWarmupTurns();
	combatant->State = ActionState::Start;
}

bool ASignalsBattleMode::updateCombatant( UWorld * world, Combatant * combatant, float dt )
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
				if (selectActionAI(this,combatant))
				{
					auto action = combatant->Activity->GetAction();
					combatant->State = ActionState::Paused;
					_nextState = ActionState::Start;
					_pauseTimer = 2.0f; 
					_infoIcon = action->GetMenuIcon();
					_infoText = FString::Printf(TEXT("%s: %s"), *combatant->Avatar->GetName(), *action->GetName());
					UpdateUI();
					AddSpeechBubble(combatant->Avatar, action->GetName(), _pauseTimer);
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

		case ActionState::Paused:
			_pauseTimer -= dt;
			if (_pauseTimer <= 0)
			{
				combatant->State = _nextState;
			}
			break;

		case ActionState::Waiting:
			if (combatant->IsHuman && _boostGaugeActive)
			{
				_boostTime -= dt;
				if (_boostTime <= 0.0)
				{
					_boostGaugeActive = false;
					_boostTime = 0;
				}
			}
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
			if (combatant->Activity->IsFinished())
			{
				combatant->State = ActionState::Complete;
			}
			break;

		case ActionState::Complete:
			UE_LOG(SignalsLog, Log, TEXT("%s turn complete"), *combatant->Avatar->GetName());
			delete combatant->Activity;
			combatant->Activity = nullptr;
			combatant->State = ActionState::Idle;
			advance = true;
			break;

		case ActionState::SkipTurn:
			UE_LOG(SignalsLog, Log, TEXT("%s skipping turn"), *combatant->Avatar->GetName());
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

void ASignalsBattleMode::HandleUseItem( int itemID )
{
	UE_LOG(SignalsLog, Log, TEXT("ASignalsBattleMode::HandleUseItem()"));

	auto player = &_combatants[_currentPlayerIndex];
	auto stats = Cast<UHumanPlayerStats>(player->Stats);
	auto & inventory = stats->GetInventory();
	inventory.RemoveItem(itemID);
	auto item = Item::FindByID(itemID);
	check(item != nullptr);
	item->Use(this,player);
}

void ASignalsBattleMode::InvokeAction(FString const & actionName)
{
	TMap<FString, FString> emptyArgs;
	InvokeAction(actionName, emptyArgs);
}

void ASignalsBattleMode::InvokeAction( FString const & actionName, TMap<FString,FString> const & args )
{
	UE_LOG(SignalsLog, Log, TEXT("Action %s selected"), *actionName);

	auto action = Action::FindAction(actionName);
	checkf(action != nullptr, TEXT("No such action: %s"), *action);
	_actionArgs = args;
	_targets.Empty();
	auto player = &_combatants[_currentPlayerIndex];
	if (action->AffectsMultipleTargets())
	{
		// TODO: consider manual selection of either side rather than auto-targetting.
		if (action->GetClass() == EActionClass::Offensive)
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
		_menuState = EMenuState::SelectTarget;
		// Offensive actions should select the enemies first; otherwise Our Side.
		TArray<Combatant *> friends;
		TArray<Combatant *> enemies;
		if (action->GetClass() == EActionClass::Offensive)
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

bool ASignalsBattleMode::HandleMenuSelect(int itemID)
{
	_selectedItem = _menuItems.FindByPredicate([itemID](FActionMenuItem const & item)
	{
		return(item.ID == itemID);
	});
	if (_selectedItem->IsLeaf)
	{
		switch (_selectedItem->ItemType)
		{
			case ItemType::RunAction:
				InvokeAction(_selectedItem->Text);
				return true;

			case ItemType::UseInventoryItem:
				_menuState = EMenuState::ItemInventory;
				ShowInventory();
				_infoText = TEXT("Select an item to use");
				UpdateUI();
				return false;
		}
	}

	return false;
}

void ASignalsBattleMode::OnInventoryClosed()
{
	UE_LOG(SignalsLog, Log, TEXT("Inventory closed"));
	auto player = &_combatants[_currentPlayerIndex];
	_menuState = EMenuState::SelectAction;
	_infoText = FString::Printf(TEXT("%s: select an action"), *player->Avatar->GetName());
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

	character->SpawnDefaultController();

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
		stats = _resMgr->LoadNPCStats(actorId);
	}

	Combatant comb(start, human, character, stats);
	_combatants.Add(comb);
}

//-----------------------------------------------------------------------------

static bool selectActionAI( ASignalsBattleMode * battle, Combatant * combatant )
{
	auto stats = Cast<UNpcPlayerStats>(combatant->Stats);
	if (stats == nullptr)
	{
		return false;
	}

	auto strategy = stats->GetStrategy();
	if (strategy == nullptr)
	{
		return false;
	}

	return strategy->Run(battle, combatant);
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

static bool checkForWin(TArray<Combatant> const & players, bool human)
{
	for (int i = 0; i < players.Num(); ++i)
	{
		if ((players[i].IsHuman == human) && players[i].IsAlive)
		{
			return false;
		}
	}

	return true;
}

static void loadStaticData(UResourceManager * resMgr)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading static assets"));

	auto contentFolder = FPaths::GameContentDir();
	auto dataPath = FPaths::Combine(*contentFolder, TEXT("Data/battle-static.xml"));
	FXmlFile dataXml;
	if (!dataXml.LoadFile(dataPath, EConstructMethod::Type::ConstructFromFile))
	{
		UE_LOG(SignalsLog, Error, TEXT("Error loading battle-static.xml"));
	}

	auto root = dataXml.GetRootNode();
	auto & children = root->GetChildrenNodes();
	for (auto itemNode : children)
	{
		auto typeStr = itemNode->GetAttribute(TEXT("type"));
		typeStr.ToLowerInline();
		auto name = itemNode->GetAttribute(TEXT("name"));
		if (typeStr == TEXT("fx"))
		{
			resMgr->LoadParticleSystem(name);
		}
		// other types here.
	}
}
