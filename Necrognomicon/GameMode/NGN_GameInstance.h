// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Managers/NGN_GnomeManager.h"
#include "Managers/NGN_MusicManager.h"
#include "Managers/NGN_ScoreManager.h"
#include "Managers/VideoTutorialManager.h"
#include "Managers/OptionsMenuMnager.h"
#include "NGN_GameInstance.generated.h"


UENUM(BlueprintType)
enum class ELanguage : uint8
{
	English
	,Spanish
	//,Italian
	//,French
};


UENUM(BlueprintType)
enum class EInputDevice : uint8
{
	Keyboard
	, Gamepad
};

UENUM(BlueprintType)
enum class EGameModeLock : uint8
{
	Default
	, Locked
	, Unlocked
};

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	void Init() override;

	UFUNCTION(BlueprintCallable)
	UNGN_GnomeManager* GetGnomeManager() const;

	UFUNCTION(BlueprintCallable)
	UNGN_MusicManager* GetMusicManager() const;

	UFUNCTION(BlueprintCallable)
	UNGN_ScoreManager* GetScoreManager() const;
	
	UFUNCTION(BlueprintCallable)
	UVideoTutorialManager* GetTutorialManager() const;
	
	UFUNCTION(BlueprintCallable)
	UOptionsMenuMnager* GetOptionsMenuManager() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Localization")
		ELanguage SelectedLanguage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EInputDevice currentInputDevice;

	UFUNCTION(BlueprintCallable)
		EInputDevice GetCurrentInputDevice() const;

	UFUNCTION(BlueprintCallable)
	void CheckInputDevice();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGameModeLock gameModeLock;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UNGN_GnomeManager> _gnomeManagerBPClassName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UNGN_MusicManager> _musicManagerBPClassName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UNGN_ScoreManager> _scoreManagerBPClassName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UVideoTutorialManager> _tutorialManagerBPClassName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UOptionsMenuMnager> __optionsMenuBPClassName;

	UPROPERTY()
	UNGN_GnomeManager* _gnomeManager;

	UPROPERTY()
	UNGN_MusicManager* _musicManager;

	UPROPERTY()
	UNGN_ScoreManager* _scoreManager;
	
	UPROPERTY()
	UVideoTutorialManager* _tutorialManager;
	
	UPROPERTY()
	UOptionsMenuMnager* _optionsManager;

	UPROPERTY()
	APlayerController* pc = nullptr;
};
