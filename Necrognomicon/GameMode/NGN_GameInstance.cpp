// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_GameInstance.h"
#include "Engine/World.h"
#include "Input/Events.h"
#include "GameFramework/PlayerController.h"

void UNGN_GameInstance::Init()
{

	_gnomeManager = NewObject<UNGN_GnomeManager>(this, _gnomeManagerBPClassName);
	_musicManager = NewObject<UNGN_MusicManager>(this, _musicManagerBPClassName);
	_scoreManager = NewObject<UNGN_ScoreManager>(this, _scoreManagerBPClassName);
	_tutorialManager = NewObject<UVideoTutorialManager>(this, _tutorialManagerBPClassName);
	_optionsManager = NewObject<UOptionsMenuMnager>(this, __optionsMenuBPClassName);
	currentInputDevice = EInputDevice::Keyboard;
}

UNGN_GnomeManager* UNGN_GameInstance::GetGnomeManager() const
{
	return _gnomeManager;
}

UNGN_MusicManager* UNGN_GameInstance::GetMusicManager() const
{
	return _musicManager;
}

UNGN_ScoreManager* UNGN_GameInstance::GetScoreManager() const
{
	return _scoreManager;
}

UVideoTutorialManager* UNGN_GameInstance::GetTutorialManager() const
{
	return _tutorialManager;
}

UOptionsMenuMnager* UNGN_GameInstance::GetOptionsMenuManager() const
{
	return _optionsManager;
}


EInputDevice UNGN_GameInstance::GetCurrentInputDevice() const
{
	return currentInputDevice;
}


void UNGN_GameInstance::CheckInputDevice()
{
	// Retrieve player controller
	if (pc == nullptr) {
		pc = GetWorld()->GetFirstPlayerController();
	}

	////////////////////////////////////////////////////////////////////////////////
	// Ccomprobacion del movimiento del raton
	////////////////////////////////////////////////////////////////////////////////
	float DeltaX = 0;
	float DeltaY = 0;
	pc->GetInputMouseDelta(DeltaX, DeltaY);
	if (DeltaX != 0 || DeltaY != 0 && currentInputDevice == EInputDevice::Gamepad) {
		//UE_LOG(LogTemp, Warning, TEXT("Check movimiento raton"));
		currentInputDevice = EInputDevice::Keyboard;
		return;
	}



	// List with all the possible keys
	TArray<FKey> keys;
	EKeys::GetAllKeys(keys);


	// Activation threshold values
	bool axisThresholdExceded = false;
	float floatAxisMin = 0.1;

	float axisValue = 0;
	FKey a = EKeys::AnyKey;

	//Check key status
	if (keys.Num() > 0) {

		////////////////////////////////////////////////////////////////////////////////
		// Comprobar tan solo los indices asociados al GamePad
		////////////////////////////////////////////////////////////////////////////////
		if (currentInputDevice != EInputDevice::Gamepad) {
			for (int32 Index = 127; Index <= 156; ++Index) {
				a = keys[Index];
				axisThresholdExceded = false;

				if (pc->WasInputKeyJustPressed(a)) {
					if (a.IsFloatAxis()) {

						bool bFound = false;
						for (const FInputAxisKeyBinding& AxisBinding : pc->InputComponent->AxisKeyBindings)
						{
							if (AxisBinding.AxisKey == a)
							{
								bFound = true;
								break;
							}
						}
						axisValue = bFound ? pc->GetInputAxisKeyValue(a) : pc->GetInputAnalogKeyState(a);
						axisThresholdExceded = bFound ? axisValue > floatAxisMin : false;
					}
					else {
						axisValue = 0;
						axisThresholdExceded = true;
					}

					if (axisThresholdExceded) {
						currentInputDevice = EInputDevice::Gamepad;
						//UE_LOG(LogTemp, Warning, TEXT("INPUT DEVICE CHANGED TO GAMEPAD"));
						break;
					}
				}
			}
		}
		else {
			////////////////////////////////////////////////////////////////////////////////
			// Comprobar la pulsación del teclado y el ratón
			////////////////////////////////////////////////////////////////////////////////
			//UE_LOG(LogTemp, Warning, TEXT("Check pulsacion teclado y raton"));			
			for (int32 Index = 1; Index <= 126; ++Index) {
				a = keys[Index];
				axisThresholdExceded = false;

				if (pc->WasInputKeyJustPressed(a)) {
					if (a.IsFloatAxis()) {

						bool bFound = false;
						for (const FInputAxisKeyBinding& AxisBinding : pc->InputComponent->AxisKeyBindings)
						{
							if (AxisBinding.AxisKey == a)
							{
								bFound = true;
								break;
							}
						}
						axisValue = bFound ? pc->GetInputAxisKeyValue(a) : pc->GetInputAnalogKeyState(a);
						axisThresholdExceded = bFound ? axisValue > floatAxisMin : false;
					}
					else {
						axisValue = 0;
						axisThresholdExceded = true;
					}

					if (axisThresholdExceded) {
						currentInputDevice = EInputDevice::Keyboard;
						break;
					}
				}
			}
		}
	}
}
