// (c) Smoke and Mirrors Software 2016

#pragma once

#include "SignalsGameMode.h"
#include "Combatant.h"
#include "Scheduler.h"
#include "ActionMenuItem.h"
#include "SignalsBattleMode.generated.h"

class Action;
class ActionInstance;
class Random;
class ResourceManager;

UENUM(BlueprintType)
enum class MenuState
{
	SelectAction,
	SelectTarget,
};

/**
* Battle mode of the game. Turn-based fighting action.
*/
UCLASS()
class SIGNALS_API ASignalsBattleMode : public ASignalsGameMode
{
	GENERATED_BODY()
public:
	void InitializeInput(UInputComponent * input) override;
	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type reason) override;
	void Tick(float dt) override;

	// Enables or disables player control.
	void EnablePlayerInput(bool enable,bool force=false);

	// Is the currently active character one of the humans?
	UFUNCTION(BlueprintPure, Category = "Battle State")
	bool IsActiveCombatantHuman() const;

	// Returns the name of the human player by index.
	UFUNCTION(BlueprintPure, Category = "Battle State")
	FString GetHumanPlayer(int index) const;

	// Returns name of active character.
	UFUNCTION(BlueprintPure, Category = "Battle State")
	FString GetActiveCombatant() const;

	UFUNCTION(BlueprintPure, Category = "Battle State")
	TArray<ACharacter *> & GetHumanPlayers();

	// Gets all the combatants in the battle.
	// TODO: make them pointers.
	TArray<Combatant> & GetAllCombatants();
	TArray<Combatant> const & GetAllCombatants() const;

	// If true, commands can be selected from the UI.
	UFUNCTION(BlueprintPure, Category = "Battle State")
	bool CanSelectCommands() const;

	// Cycles through the battle cameras.
	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void CycleCameras();

	// Switches to the given camera by index.
	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void SwitchToCamera(int camera);

	// Called when a character's turn begins.
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle State")
	void OnTurnBeginning(ACharacter * character, bool isHuman);

	// Adds a speech bubble that fades over the specified time.
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void AddSpeechBubble(ACharacter * character, FString const & text, float time);

	// Refresh the HUD.
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateUI();

	// Menu events.
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuLeft();
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuRight();
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuSelect();
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuBack();

	// Called by animation / motion code when a command completes.
	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionComplete();

	// Adds a floating text notification for (eg) HP loss.
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle State")
	void AddFloatingNotification( ACharacter * who, FString const & text, FVector color );

	// Sets the action for the current player.
	void SetCurrentCombatantAction(ActionInstance * action);
	
	// Gets the available actions for the current player.
	UFUNCTION(BlueprintCallable, Category = "UI")
	TArray<FActionMenuItem> GetAvailableActions( int level ) const;

	// Callback for menu selection.
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandleMenuSelect(int itemID);

	// Plays a named animation on the character, and optional sound.
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle State")
	void PlayAnimation(ACharacter * character, FString const & animation, USoundWave * sound, bool notify);

	// Picking actions or sub-actions?
	UFUNCTION(BlueprintPure, Category = "UI")
	MenuState GetMenuState() const;

	// Marker API - hovering arrows to show current player / target.
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowActiveMarker();
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void HideActiveMarker();
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowTargetMarker(ACharacter * target);
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void HideTargetMarker();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PreviousTarget();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void NextTarget();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CancelTarget();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SelectTarget();

	// Gets the text for the info bar.
	UFUNCTION(BlueprintPure, Category = "UI")
	FString GetInfoText() const;

	// Gets the icon index for the info bar.
	UFUNCTION(BlueprintPure, Category = "UI")
	int GetInfoIcon() const;

	// Is the boost control active for the current player?
	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsBoostActive() const;

	// Gets the fraction of boost left.
	UFUNCTION(BlueprintPure, Category = "UI")
	float GetBoostFraction() const;

	// Called when the action system requires payload delivery.
	void RunActionPayload();

	// Call to apply damage to targets.
	void ApplyDamage();

	// Gets the source and target(s) of an action.
	Combatant * GetActionSource();
	Combatant const * GetActionSource() const;
	TArray<Combatant *> const & GetActionTargets() const;
	void SetActionTargets(TArray<Combatant *> const & targets);
	TArray<Combatant *> GetActiveHumans() const;
	TArray<Combatant *> GetActiveNPCs() const;

	// Gets the random number generator for the battle.
	Random * GetRandom() const;

	// Gets the resource manager.
	ResourceManager * GetResourceManager();

private:
	bool updateCombatant(UWorld * world, Combatant * combatant,float dt);
	void nextTurn( bool firstTurn );
	void initCombatant(UWorld * world, APlayerStart * start, FString actorId, bool human);
	void initCombatants(UWorld * world, APlayerStart * starts[], TArray<FString> const & combatants, bool human);
	void findAvailableActions(Combatant * const combatant);
	void scheduleTurn(int playerIndex);
	void refreshTargetInfo();

	enum { MAX_PLAYER_STARTS = 3 };
	APlayerStart * _playerStarts[MAX_PLAYER_STARTS];
	APlayerStart * _enemyStarts[MAX_PLAYER_STARTS];
	TArray<Combatant> _combatants;
	TArray<ACharacter *> _players;
	TArray<ACharacter *> _enemies;
	TArray<ACameraActor *> _cameras;
	int _currentPlayerIndex;
	float _cameraSwitchTimer;
	int _cameraIndex;
	Scheduler _scheduler;
	bool _commandsEnabled;
	TArray<FActionMenuItem> _menuItems;
	FActionMenuItem * _selectedItem;
	MenuState _menuState;
	Action * _selectedAction;
	TArray<Combatant *> _targets;
	int _currentTarget;
	FString _infoText;
	int _infoIcon;
	bool _boostGaugeActive;
	float _boostMaxTime;
	float _boostTime;
	ResourceManager * _resMgr;
	ActionState _nextState;
	float _pauseTimer;
};

inline Combatant const * ASignalsBattleMode::GetActionSource() const
{
	return(&_combatants[_currentPlayerIndex]);
}

inline Combatant * ASignalsBattleMode::GetActionSource()
{
	return(&_combatants[_currentPlayerIndex]);
}

inline TArray<Combatant *> const & ASignalsBattleMode::GetActionTargets() const
{
	return(_targets);
}

inline ResourceManager * ASignalsBattleMode::GetResourceManager()
{
	return _resMgr;
}

inline TArray<Combatant> & ASignalsBattleMode::GetAllCombatants()
{
	return _combatants;
}

inline TArray<Combatant> const & ASignalsBattleMode::GetAllCombatants() const
{
	return _combatants;
}