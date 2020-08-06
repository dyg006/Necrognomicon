// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Managers/NGN_MusicManager.h"
#include "Tutorial/NecrognomiconGate.h"
#include "FMODBlueprintStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "PlayerRespawn/PlayerRespawn.h"
#include "Character/NecrognomiconController.h"
#include "Buildings/House.h"
#include "NecrognomiconGameMode.generated.h"

class UNGN_ScoreManager;
class UVideoTutorialManager;
class UNGN_GnomeManager;


UCLASS()
class ANecrognomiconGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* TitleScreenWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* NecroWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* WinWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* LoseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* PauseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* OptionsWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* GlosaryWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* TrapPlacementWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Tutorial")
		bool isTutorialDoorClosed = true;

	UPROPERTY(BlueprintReadWrite, Category = "Tutorial")
		int numberOfItemsPicked = 0;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void CreateWinWidget();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void CreateLoseWidget();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void FinishGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void RestartGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void RetryGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OpenOptionsMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OpenGlosary();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OpenBackPauseMenu();

	void createPauseMenu();

	void CreateTrapPlacementWidget();

	void SetVisibilityTrapPlacementWidget(bool visibility);

	/// <summary>
	/// Respawns the player.
	/// </summary>
	void RespawnPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = playerSpawn)
	/// <summary>
	/// The spawn player transform
	/// </summary>
	FTransform spawnPlayerTransform;

	UPROPERTY(BlueprintReadWrite)
	/// <summary>
	/// The spawn points
	/// </summary>
	TArray<APlayerRespawn*>		spawnPoints;

	UPROPERTY(BlueprintReadWrite)
	/// <summary>
	/// The gates
	/// </summary>
	TArray<ANecrognomiconGate*> gates;

	/// <summary>
	/// The spawn points
	/// </summary>
	AHouse* currentLevelHouse;

	UFUNCTION(BlueprintCallable)
	bool GetIsPlayerDead() { return isPlayerDead; };

	void SetIsPlayerDead(bool playerDead);

	virtual void ResetLevel() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ResetCurrentLevel();


	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void LoadStartScreen();

	UPROPERTY(EditAnywhere, Category = "Sounds")
	UFMODEvent* gameOverSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	UFMODEvent* victorySound;

	/// <summary>
	/// Called when the game is over
	/// </summary>
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "Score")
	/// <summary>
	/// Increases the score.
	/// </summary>
	void IncreaseScore( int score = 1 );

	UFUNCTION(BlueprintImplementableEvent)
	void SetPathsVisibility(bool Visible);


	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The first game intensity in order to activate first track transition 
	/// </summary>
	float gameFlagIntensity = 0.f;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The second game intensity in order to activate combat track of the first level
	/// </summary>
	float gameFlagIntensity2 = 21.f;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The third game intensity in order to activate the combat track of the second level
	/// </summary>
	float gameFlagIntensity3 = 41.f;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The fourth game intensity in order to activate the combat track of the third level
	/// </summary>
	float gameFlagIntensity4 = 61.f;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The third game intensity in order to activate the combat track of the fourth level
	/// </summary>
	float gameFlagIntensity5 = 81.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int CurrentLevel = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	bool IsLevelHordeActive = false;

protected:

	ANecrognomiconGameMode();
	void StartPlay() override;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIWinWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UILoseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIPauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIOptionsWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIGlosaryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UITrapPlacementWidget;

	/// <summary>
	/// The main character
	/// </summary>
	ANecrognomiconCharacter* necroCharacter;

	/// <summary>
	/// Check if the player is dead
	/// </summary>
	bool isPlayerDead = false;

	/// <summary>
	/// The game is lost
	/// </summary>
	bool gameLost;

	/// <summary>
	/// The number of gnomes destroyed
	/// </summary>
	int gnomesDestroyed = 0;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	int numberOfTutorialGnomes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial")
	int numberOfTutorialItems = 2;

	void AddMatchRow();

	void AddMatchRow(FString Event);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Score")
	float GetHouseHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Score")
	float GetDifficulty();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Score")
	bool GetHordeActive();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Score")
	FString GetCurrentWeapn();

	UFUNCTION(BlueprintCallable, Category = "SlotManager")
	void SetPlayerSlots();

private:
	ANecrognomiconController* playerController;
	UUserWidget* CurrentWidget;

	void StartGame();

	/// <summary>
	/// The music manager
	/// </summary>
	UNGN_MusicManager* musicManager;

	/// <summary>
	/// Score manager
	/// </summary>
	UNGN_ScoreManager* scoreManager;

	/// <summary>
	/// Gnome manager
	/// </summary>
	USlotManagerComponent* slotManager;

	/// <summary>
	/// Score manager
	/// </summary>
	UVideoTutorialManager* tutorialManager;

	/// <summary>
	/// Gnome manager
	/// </summary>
	UNGN_GnomeManager* gnomeManager;
};