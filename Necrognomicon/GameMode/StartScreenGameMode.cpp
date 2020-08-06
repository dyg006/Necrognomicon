// Fill out your copyright notice in the Description page of Project Settings.


#include "StartScreenGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


AStartScreenGameMode::AStartScreenGameMode()
{
}

void AStartScreenGameMode::StartPlay()
{
    Super::StartPlay();
}

void AStartScreenGameMode::BeginPlay()
{
    Super::BeginPlay();

    worldController = GetWorld()->GetFirstPlayerController();
    CreateTitleScreen();
}

void AStartScreenGameMode::CreateTitleScreen()
{
    if (UTitleScreenWidget)
    {
        if (TitleScreenWidget) {
            TitleScreenWidget->RemoveFromParent();
            TitleScreenWidget = nullptr;
        }
        TitleScreenWidget = CreateWidget<UUserWidget>(worldController, UTitleScreenWidget);
        TitleScreenWidget->AddToViewport();
        TitleScreenWidget->SetKeyboardFocus();
        worldController->bShowMouseCursor = true;
    }
}

void AStartScreenGameMode::LoadFirstLevel()
{
    FInputModeGameOnly inputModeGameOnly;
    inputModeGameOnly.SetConsumeCaptureMouseDown(false);
    worldController->SetInputMode(inputModeGameOnly);
    worldController->bShowMouseCursor = false;

    TitleScreenWidget->RemoveFromParent();
 
    FString levelName = "BaseLevel";
    // options> difficulty (easy, normal, hard)
    FString difficultyOption = "difficulty=hard";
    FName currentLevel = FName(*levelName);
    UGameplayStatics::OpenLevel(this, currentLevel, true, difficultyOption);
}

void AStartScreenGameMode::LoadEndlessLevel()
{
    FInputModeGameOnly inputModeGameOnly;
    inputModeGameOnly.SetConsumeCaptureMouseDown(false);
    worldController->SetInputMode(inputModeGameOnly);
    worldController->bShowMouseCursor = false;

    TitleScreenWidget->RemoveFromParent();

    FString levelName = "EndlessLevel";
    FName currentLevel = FName(*levelName);
    UGameplayStatics::OpenLevel(this, currentLevel);
}

void AStartScreenGameMode::FinishGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), worldController,
        EQuitPreference::Quit, false);
}


void AStartScreenGameMode::LoadOptionsMenu()
{
    if (UTitleScreenWidget)
    {
        if (TitleScreenWidget) {
            TitleScreenWidget->RemoveFromParent();
            TitleScreenWidget = nullptr;
        }
        TitleScreenWidget = CreateWidget<UUserWidget>(worldController, UIOptionsWidget);
        TitleScreenWidget->AddToViewport();
        TitleScreenWidget->SetKeyboardFocus();

        //playerController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
        //playerController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        //playerController->inputModeGameAndUI.SetWidgetToFocus(PauseWidget->TakeWidget());
        //playerController->SetInputMode(playerController->inputModeGameAndUI);
        //playerController->bShowMouseCursor = true;
    }
}

void AStartScreenGameMode::LoadCredits()
{
    if (UTitleScreenWidget)
    {
        if (TitleScreenWidget) {
            TitleScreenWidget->RemoveFromParent();
            TitleScreenWidget = nullptr;
        }
        TitleScreenWidget = CreateWidget<UUserWidget>(worldController, UICreditsWidget);
        TitleScreenWidget->AddToViewport();
        TitleScreenWidget->SetKeyboardFocus();

        //playerController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
        //playerController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        //playerController->inputModeGameAndUI.SetWidgetToFocus(PauseWidget->TakeWidget());
        //playerController->SetInputMode(playerController->inputModeGameAndUI);
        //playerController->bShowMouseCursor = true;
    }
}


void AStartScreenGameMode::UnloadOptionsMenu()
{
    CreateTitleScreen();
}

void AStartScreenGameMode::LoadAdventureMenu()
{
    if (UTitleScreenWidget)
    {
        if (TitleScreenWidget) {
            TitleScreenWidget->RemoveFromParent();
            TitleScreenWidget = nullptr;
        }
        TitleScreenWidget = CreateWidget<UUserWidget>(worldController, UIAdventureWidget);
        TitleScreenWidget->AddToViewport();
        TitleScreenWidget->SetKeyboardFocus();
    }
}

void AStartScreenGameMode::LoadEndlessMenu()
{
    if (UTitleScreenWidget)
    {
        if (TitleScreenWidget) {
            TitleScreenWidget->RemoveFromParent();
            TitleScreenWidget = nullptr;
        }
        TitleScreenWidget = CreateWidget<UUserWidget>(worldController, UIEndlessWidget);
        TitleScreenWidget->AddToViewport();
        TitleScreenWidget->SetKeyboardFocus();
    }
}

void AStartScreenGameMode::BackToMainMenu()
{
    CreateTitleScreen();
}

void AStartScreenGameMode::LoadFirstLevelWithMode(FString mode)
{
    FInputModeGameOnly inputModeGameOnly;
    inputModeGameOnly.SetConsumeCaptureMouseDown(false);
    worldController->SetInputMode(inputModeGameOnly);
    worldController->bShowMouseCursor = false;

    TitleScreenWidget->RemoveFromParent();

    FString levelName = "BaseLevel";
    // options> difficulty (easy, normal, hard)
    FString difficultyOption = "difficulty=" + mode;
    FName currentLevel = FName(*levelName);
    UGameplayStatics::OpenLevel(this, currentLevel, true, difficultyOption);
}
