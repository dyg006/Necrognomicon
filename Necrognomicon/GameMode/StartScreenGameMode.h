// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "StartScreenGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API AStartScreenGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* TitleScreenWidget;

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadFirstLevel();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadEndlessLevel();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void FinishGame();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadOptionsMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void UnloadOptionsMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadAdventureMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadEndlessMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void BackToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadFirstLevelWithMode(FString mode);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void LoadCredits();

protected:

	AStartScreenGameMode();
	void StartPlay() override;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UTitleScreenWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIOptionsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UICreditsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIAdventureWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> UIEndlessWidget;

private:
	APlayerController* worldController;
	void CreateTitleScreen();
};
