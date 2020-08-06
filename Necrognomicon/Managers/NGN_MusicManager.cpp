// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_MusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"
#include "fmod_studio.hpp"


UNGN_MusicManager::UNGN_MusicManager()
{
}

UNGN_MusicManager::~UNGN_MusicManager()
{
}

void UNGN_MusicManager::PlayMainMusicLevel(AActor * mainCharacter)
{
	UFMODBlueprintStatics::BusSetVolume(busAllSound, 0.7f);
	if (levelMusicSound != nullptr && mainCharacter != nullptr) {
		levelMusicSoundInstance = UFMODBlueprintStatics::PlayEventAttached(levelMusicSound, mainCharacter->GetRootComponent(), "", mainCharacter->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
	}
}

void UNGN_MusicManager::SetGameIntensityParam(float intensity) {
	if (levelMusicSoundInstance != nullptr) {
		levelMusicSoundInstance->SetParameter("Intensity", intensity);
	}
}

void UNGN_MusicManager::TurnOffGeneralVolume(AActor* mainCharacter)
{
	if (turnOffMusicEvent != nullptr && mainCharacter != nullptr) {
		generalVolumeInstance = UFMODBlueprintStatics::PlayEventAttached(turnOffMusicEvent, mainCharacter->GetRootComponent(), "", mainCharacter->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
	}
}

void UNGN_MusicManager::TurnOnGeneralVolume()
{
	if (generalVolumeInstance != nullptr) {
		generalVolumeInstance->Stop();
	}
}

//UFMODAudioComponent* UNGN_MusicManager::PlayDiscoMushroomMusic(AActor* const& discoMushroom, UFMODEvent* const& soundEvent) {
//	if (discoMushroomInstance != nullptr) {
//		if (discoMushroom != nullptr) {
//			//discoMushroomInstance = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("DiscoMushroomSound"));
//			//discoMushroomInstance = UFMODBlueprintStatics::PlayEventAttached(discoMushroomSound, discoMushroom->GetRootComponent(), "", discoMushroom->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
//			UE_LOG(LogTemp, Error, TEXT("Activating another discomushroom"));
//			discoMushroomInstance->bAutoActivate = true;
//			//discoMushroomInstance->Play();
//			discoMushroomInstance->SetRelativeLocation(discoMushroom->GetActorLocation());// UFMODBlueprintStatics::(discoMushroomInstance, discoMushroom->GetTransform());
//			discoMushroomInstance->SetupAttachment(discoMushroom->GetRootComponent());		
//			return discoMushroomInstance;
//		}
//	} 
//	else {
//		if (soundEvent != nullptr) {
//			discoMushroomInstance = UFMODBlueprintStatics::PlayEventAttached(soundEvent, discoMushroom->GetRootComponent(), "", discoMushroom->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
//			discoMushroomInstance->bAutoActivate = true;
//			discoMushroomInstance->SetupAttachment(discoMushroom->GetRootComponent());
//			return discoMushroomInstance;
//		}
//	}
//	return nullptr;
//
//}
