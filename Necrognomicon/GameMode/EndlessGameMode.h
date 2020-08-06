// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/NecrognomiconController.h"
#include "EndlessGameMode.generated.h"

class UNGN_MusicManager;
class UNGN_ScoreManager;
class UVideoTutorialManager;

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API AEndlessGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* NecroWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* TrapPlacementWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* ChronoWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* LoseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* OptionsWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* GlosaryWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UUserWidget* SubGoalWidget;

protected:
	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UITrapPlacementWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UIChronoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UISubGoalWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UILoseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UIPauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UIOptionsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> UIGlosaryWidget;

	ANecrognomiconCharacter* necroCharacter;

private:

	ANecrognomiconController* playerController;

	UNGN_MusicManager* musicManager;

	UNGN_ScoreManager* scoreManager;

	UVideoTutorialManager* tutorialManager;

	UUserWidget* CurrentWidget;

public:
	void SetVisibilityTrapPlacementWidget(bool visibility);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void LoadStartScreen();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void RestartGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void OpenOptionsMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void OpenGlosary();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void OpenBackPauseMenu();

	void CreateLoseWidget();

	void createPauseMenu();	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Score")
	float GetDifficulty();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Score")
	FString GetCurrentWeapn();

protected:
	AEndlessGameMode();

	// Override BeginPlay()
	virtual void BeginPlay() override;

private:
	void StartGame();

	void AddMatchRow();

	void AddMatchRow(FString Event);

};
