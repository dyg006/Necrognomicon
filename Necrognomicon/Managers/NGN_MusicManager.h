// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "FMODAudioComponent.h"
#include "UObject/NoExportTypes.h"
#include "NGN_MusicManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NECROGNOMICON_API UNGN_MusicManager : public UObject
{
	GENERATED_BODY()

public:
	UNGN_MusicManager();
	~UNGN_MusicManager();

	/// <summary>
	/// Plays the main music level.
	/// </summary>
	void PlayMainMusicLevel(AActor *mainCharacter);

	/// <summary>
	/// Turns off general volume.
	/// </summary>
	void TurnOffGeneralVolume(AActor* mainCharacter);

	/// <summary>
	/// Turns on general volume.
	/// </summary>
	void TurnOnGeneralVolume();

	/// <summary>
	/// Plays the discomushroom song.
	/// </summary>
	UFMODAudioComponent* PlayDiscoMushroomMusic(AActor* const& discoMushroom, UFMODEvent* const& soundEvent);

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Sets the game intensity parameter.
	/// </summary>
	/// <param name="intensity">The intensity.</param>
	void SetGameIntensityParam(float intensity);

	/** Level music sound with combat transitions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class UFMODEvent* levelMusicSound;

	/** Level music sound with combat transitions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class UFMODEvent* turnOffMusicEvent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class UFMODBus* busAllSound;

	UFUNCTION(BlueprintCallable)
	float GetGeneralVolumeValue() const { return generalVolumeValue; }
	UFUNCTION(BlueprintCallable)
	float GetMusicVolumeValue() const { return musicVolumeValue; }
	UFUNCTION(BlueprintCallable)
	float GetSoundEffecsVolumeValue() const { return soundEffecsVolumeValue; }
	UFUNCTION(BlueprintCallable)
	float GetAmbientSoundsVolumeValue() const { return ambientSoundsVolumeValue; }

	UFUNCTION(BlueprintCallable)
	void SetGeneralVolumeValue(const float volumeValue) { generalVolumeValue = volumeValue; }
	UFUNCTION(BlueprintCallable)
	void SetMusicVolumeValue(const float volumeValue) { musicVolumeValue = volumeValue; }
	UFUNCTION(BlueprintCallable)
	void SetSoundEffectsVolumeValue(const float volumeValue) { soundEffecsVolumeValue = volumeValue; }
	UFUNCTION(BlueprintCallable)
	void SetAmbientSoundsVolumeValue(const float volumeValue) { ambientSoundsVolumeValue = volumeValue; }

	

private:
	class UFMODAudioComponent* levelMusicSoundInstance;

	class UFMODAudioComponent* generalVolumeInstance;

	//class UFMODAudioComponent* discoMushroomInstance;

	float generalVolumeValue = 0.7f;
	
	float musicVolumeValue = 1.f;
	
	float soundEffecsVolumeValue = 1.f;
	
	float ambientSoundsVolumeValue = 1.f;

};
