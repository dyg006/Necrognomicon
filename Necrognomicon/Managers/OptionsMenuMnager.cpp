// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenuMnager.h"


#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/NGN_OptionsSaveGame.h"

#include <Windows/MinWindows.h> // Ñapa para simular que se pulsa F11 y cambiar entre pantalla completa y ventana

UOptionsMenuMnager::UOptionsMenuMnager()
{

}

UOptionsMenuMnager::~UOptionsMenuMnager()
{
}

void UOptionsMenuMnager::Save()
{
	if(UNGN_OptionsSaveGame * SaveGameInstance = Cast<UNGN_OptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UNGN_OptionsSaveGame::StaticClass())))
	{
        // Set data on the savegame object.
        SaveGameInstance->MinimapEnabled = _minimapEnabled;
        SaveGameInstance->TutorialEnabled = _tutorialEnabled;
        SaveGameInstance->Language = _language;
        SaveGameInstance->Sensitivity = _sensitivity;
        SaveGameInstance->AudioGeneral = _audioGeneral;
        SaveGameInstance->AudioMusic = _audioMusic;
        SaveGameInstance->AudioEffects = _audioEffects;
        SaveGameInstance->AudioAmbient = _audioAmbient;
        SaveGameInstance->FullScreen = _fullScreen;
        SaveGameInstance->Quality = static_cast<uint8>(_quality);

        // Save the data immediately.
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex))
        {
            // Save succeeded.
        }
	}
}

void UOptionsMenuMnager::Load()
{
	// Si es la primera vez que se ejecuta, ajustes por defecto
	if(! UGameplayStatics::DoesSaveGameExist(TEXT("OptionsSaveSlot"), 0))
	{
        _minimapEnabled = true;
        _tutorialEnabled = true;
        _language = 0;
        _sensitivity = 5.0f;
        _audioGeneral = 1.0f;
        _audioMusic = 1.0f;
        _audioEffects = 1.0f;
        _audioAmbient = 1.0f;
        _fullScreen = true;
        _quality = GraphicQuality::VERY_HIGH;
		
        Save();
	}
    // Retrieve and cast the USaveGame object to UMySaveGame.
    if (UNGN_OptionsSaveGame* LoadedGame = Cast<UNGN_OptionsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("OptionsSaveSlot"), 0)))
    {
        // The operation was successful, so LoadedGame now contains the data we saved earlier.
        _minimapEnabled = LoadedGame->MinimapEnabled;
        _tutorialEnabled = LoadedGame->TutorialEnabled;
        _language = LoadedGame->Language;
        _sensitivity = LoadedGame->Sensitivity;
        _audioGeneral = LoadedGame->AudioGeneral;
        _audioMusic = LoadedGame->AudioMusic;
        _audioEffects = LoadedGame->AudioEffects;
        _audioAmbient = LoadedGame->AudioAmbient;
        _fullScreen = LoadedGame->FullScreen;
        _quality = static_cast<GraphicQuality>(LoadedGame->Quality);
    }
}

void UOptionsMenuMnager::ApplyGraphicSettings()
{
    uint8 level = 0;
    uint8 screenPercentage = 100;
	switch (_quality)
	{
    case GraphicQuality::LOW:
        level = 0;
        screenPercentage = 50;
        break;
    case GraphicQuality::MEDIUM:
        level = 1;
        screenPercentage = 75;
        break;
    case GraphicQuality::HIGH:
        level = 2;
        screenPercentage = 100;
        break;
    case GraphicQuality::VERY_HIGH:
        level = 3;
        screenPercentage = 100;
        break;
	}

    FString fullscreen = (_fullScreen) ? "f" : "w";

    // Cambiada la llamada de ejecucion de comandos al GEngine para que funcionen
    GEngine->Exec(GetWorld(), *FString::Printf(TEXT("r.ScreenPercentage %i"), screenPercentage));
    GEngine->Exec(GetWorld(), *FString::Printf(TEXT("sg.PostProcessQuality %i"), level));
    GEngine->Exec(GetWorld(), *FString::Printf(TEXT("sg.ShadowQuality %i"), level));
    GEngine->Exec(GetWorld(), *FString::Printf(TEXT("sg.TextureQuality %i"), level));



    //////////////////////////////////////////////////////////
    // Estos comandos crashean
    //////////////////////////////////////////////////////////
    //GEngine->Exec(GetWorld(), *FString::Printf(TEXT("r.SetRes 1280x720%s"), *fullscreen));
    //GEngine->Exec(GetWorld(), *FString::Printf(TEXT("sg.EffectsQuality %i"), level));


    //////////////////////////////////////////////////////////
    // Intento futil de maximizar pantalla simulando input
    //////////////////////////////////////////////////////////
    //FViewport* Viewport = GEngine->GameViewport->Viewport;
    //GEngine->GameViewport->Viewport->GetClient()->InputKey(Viewport, 0, EKeys::LeftAlt, EInputEvent::IE_Pressed);
    //GEngine->GameViewport->Viewport->GetClient()->InputKey(Viewport, 0, EKeys::Enter, EInputEvent::IE_Pressed);
    //GEngine->GameViewport->Viewport->GetClient()->InputKey(Viewport, 0, EKeys::F11, EInputEvent::IE_Pressed);
}

void UOptionsMenuMnager::ApplySettings()
{
}

void UOptionsMenuMnager::SwitchScreenMode() const
{
    // Ñapa para simular que se pulsa F11 y cambiar entre pantalla completa y ventana
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the F11 key
    ip.ki.wVk = 0x7A; //Virtual-key code for the "F11" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.dwFlags = KEYEVENTF_KEYUP; // for key release
    SendInput(1, &ip, sizeof(INPUT));
}
