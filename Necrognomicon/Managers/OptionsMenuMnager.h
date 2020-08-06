// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameMode/NGN_GameInstance.h"
#include "UObject/NoExportTypes.h"
#include "OptionsMenuMnager.generated.h"

//enum class ELanguage : uint8;
UENUM(BlueprintType)
enum class GraphicQuality : uint8 { LOW, MEDIUM, HIGH, VERY_HIGH };

/**
 * 
 */
UCLASS(Blueprintable)
class NECROGNOMICON_API UOptionsMenuMnager : public UObject
{
	GENERATED_BODY()
	

public:
	UOptionsMenuMnager();
	~UOptionsMenuMnager();

	UFUNCTION(BlueprintCallable)
	void Save();

	UFUNCTION(BlueprintCallable)
	void Load();

	UFUNCTION(BlueprintCallable)
	void ApplyGraphicSettings();

	void ApplySettings();


protected:

	UPROPERTY(VisibleAnywhere, Category = General)
		bool _minimapEnabled = true;

	UPROPERTY(VisibleAnywhere, Category = General)
		bool _tutorialEnabled;

	UPROPERTY(VisibleAnywhere, Category = General)
		uint8 _language;

	UPROPERTY(VisibleAnywhere, Category = Input)
		float _sensitivity;

	UPROPERTY(VisibleAnywhere, Category = Audio)
		float _audioGeneral;

	UPROPERTY(VisibleAnywhere, Category = Audio)
		float _audioMusic;

	UPROPERTY(VisibleAnywhere, Category = Audio)
		float _audioEffects;

	UPROPERTY(VisibleAnywhere, Category = Audio)
		float _audioAmbient;

	UPROPERTY(VisibleAnywhere, Category = Video)
		bool _fullScreen;

	UPROPERTY(VisibleAnywhere, Category = Video)
	GraphicQuality _quality = GraphicQuality::VERY_HIGH;

public:
	
	UFUNCTION(BlueprintCallable)
	bool IsMinimapEnabled() const { return _minimapEnabled; }

	UFUNCTION(BlueprintCallable)
	void SetMinimapEnabled(bool bIsMinimapEnabled) { _minimapEnabled = bIsMinimapEnabled; }

	UFUNCTION(BlueprintCallable)
	bool IsTutorialEnabled() const { return _tutorialEnabled; }

	UFUNCTION(BlueprintCallable)
	void SetTutorialEnabled(bool bTutorialEnabled) { _tutorialEnabled = bTutorialEnabled; }

	UFUNCTION(BlueprintCallable)
		uint8 GetLanguage() const { return _language; }

	UFUNCTION(BlueprintCallable)
	void SetLanguage(uint8 Language) { _language = Language; }

	UFUNCTION(BlueprintCallable)
	float GetSensitivity() const { return _sensitivity; }

	UFUNCTION(BlueprintCallable)
	void SetSensitivity(float bSensitivity) { _sensitivity = bSensitivity; }

	UFUNCTION(BlueprintCallable)
	float GetAudioGeneral() const { return _audioGeneral; }

	UFUNCTION(BlueprintCallable)
	void SetAudioGeneral(float volume) { _audioGeneral = volume; }

	UFUNCTION(BlueprintCallable)
	float GetAudioMusic() const { return _audioMusic; }

	UFUNCTION(BlueprintCallable)
	void SetAudioMusic(float volume) { _audioMusic = volume; }

	UFUNCTION(BlueprintCallable)
	float GetAudioEffects() const { return _audioEffects; }

	UFUNCTION(BlueprintCallable)
	void SetAudioEffects(float volume) { _audioEffects = volume; }

	UFUNCTION(BlueprintCallable)
	float GetAudioAmbient() const { return _audioAmbient; }

	UFUNCTION(BlueprintCallable)
	void SetAudioAmbient(float volume) { _audioAmbient = volume; }

	UFUNCTION(BlueprintCallable)
	bool IsFullScreen() const { return _fullScreen; }

	UFUNCTION(BlueprintCallable)
	void SetFullScreen(bool bFullScreen) { _fullScreen = bFullScreen; }

	UFUNCTION(BlueprintCallable)
	GraphicQuality GetGraphicQuality() const { return _quality; }

	UFUNCTION(BlueprintCallable)
	void SetGraphicQuality(GraphicQuality quality) { _quality = quality; }


	UFUNCTION(BlueprintCallable)
		void SwitchScreenMode() const;

};
