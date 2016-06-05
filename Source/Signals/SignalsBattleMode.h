// (c) Smoke and Mirrors Software 2016

#pragma once

#include "SignalsGameMode.h"
#include "Combatant.h"
#include "Scheduler.h"
#include "ActionMenuItem.h"
#include "SignalsBattleMode.generated.h"

class Action;
class ActionInstance;

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

	// If true, commands can be selected from the UI.
	UFUNCTION(BlueprintPure, Category = "Battle State")
	bool CanSelectCommands() const;

	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void CycleCameras();

	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void SwitchToCamera(int camera);

	UFUNCTION(BlueprintImplementableEvent, Category = "Battle State")
	void OnTurnBeginning(ACharacter * character, bool isHuman);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuLeft();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuRight();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuSelect();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnMenuBack();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionComplete();

	// Adds a floating text notification for (eg) HP loss.
	UFUNCTION(BlueprintImplementableEvent, Category = "Battle State")
	void AddFloatingNotification( ACharacter * who, FString const & text, FVector color );

	void SetCurrentCombatantAction(ActionInstance * action);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	TArray<FActionMenuItem> GetAvailableActions( int level ) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandleMenuSelect(int itemID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Battle State")
	void PlayAnimation(ACharacter * character, FString const & animation, USoundWave * sound);

	UFUNCTION(BlueprintPure, Category = "UI")
	MenuState GetMenuState() const;

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

	UFUNCTION(BlueprintPure, Category = "UI")
	FString GetInfoText() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	int GetInfoIcon() const;

	// Called when the action system requires payload delivery.
	void RunActionPayload();

	// Gets the source and target(s) of an action.
	Combatant const * GetActionSource() const;
	TArray<Combatant *> const & GetActionTargets() const;

private:
	bool updateCombatant(UWorld * world, Combatant * combatant,float dt);
	void nextTurn();
	void initCombatant(UWorld * world, APlayerStart * start, FString actorId, bool human);
	void initCombatants(UWorld * world, APlayerStart * starts[], TArray<FString> const & combatants, bool human);
	void findAvailableActions(Combatant * const combatant);

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
	bool _firstTurn;
	TArray<FActionMenuItem> _menuItems;
	FActionMenuItem * _selectedItem;
	MenuState _menuState;
	Action * _selectedAction;
	TArray<Combatant *> _targets;
	int _currentTarget;
	FString _infoText;
	int _infoIcon;
};

inline Combatant const * ASignalsBattleMode::GetActionSource() const
{
	return(&_combatants[_currentPlayerIndex]);
}

inline TArray<Combatant *> const & ASignalsBattleMode::GetActionTargets() const
{
	return(_targets);
}
