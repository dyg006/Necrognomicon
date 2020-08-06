// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
//#include "GameMode/NGN_GameInstance.h"

#include "NGN_OptionsSaveGame.generated.h"

//enum class ELanguage : uint8;

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_OptionsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UNGN_OptionsSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	
	UPROPERTY(VisibleAnywhere, Category = General)
		bool MinimapEnabled;

	UPROPERTY(VisibleAnywhere, Category = General)
		bool TutorialEnabled;

	UPROPERTY(VisibleAnywhere, Category = General)
		uint8 Language;

	UPROPERTY(VisibleAnywhere, Category = Input)
		float Sensitivity;

	UPROPERTY(VisibleAnywhere, Category = Audio)
		float AudioGeneral;
	
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float AudioMusic;
	
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float AudioEffects;
	
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float AudioAmbient;

	UPROPERTY(VisibleAnywhere, Category = Video)
		bool FullScreen;

	UPROPERTY(VisibleAnywhere, Category = General)
		uint8 Quality;
	
};
