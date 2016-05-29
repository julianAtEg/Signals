// (c) Smoke and Mirrors Software 2016

#pragma once

#include "SignalsGameMode.h"
#include "Combatant.h"
#include "Scheduler.h"
#include "ActionMenuItem.h"
#include "SignalsBattleMode.generated.h"

class ActionInstance;

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
	void SwitchToCamera(int camera);

	UFUNCTION(BlueprintNativeEvent, Category = "Battle State")
	void OnTurnBeginning(ACharacter * character, bool isHuman);
	void OnTurnBeginning_Implementation(ACharacter * character, bool isHuman);

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void UpdateUI();
	void UpdateUI_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnMenuLeft();
	void OnMenuLeft_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnMenuRight();
	void OnMenuRight_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnMenuSelect();
	void OnMenuSelect_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnMenuBack();
	void OnMenuBack_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Battle State")
	void OnActionComplete();

	void SetCurrentCombatantAction(ActionInstance * action);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	TArray<FActionMenuItem> GetAvailableActions( int level ) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandleActionSelect(int actionID);

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
};
