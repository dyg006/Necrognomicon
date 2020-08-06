// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessGameMode.h"
#include "NGN_GameInstance.h"
#include "Character/NecrognomiconCharacter.h"
#include "Managers/NGN_MusicManager.h"
#include "Managers/NGN_ScoreManager.h"
#include "Managers/VideoTutorialManager.h"

AEndlessGameMode::AEndlessGameMode()
{
}

void AEndlessGameMode::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* worldController = GetWorld()->GetFirstPlayerController();
    playerController = Cast<ANecrognomiconController>(worldController);

    musicManager = Cast<UNGN_GameInstance>(GetGameInstance())->GetMusicManager();
    scoreManager = Cast<UNGN_GameInstance>(GetGameInstance())->GetScoreManager();
    tutorialManager = Cast<UNGN_GameInstance>(GetGameInstance())->GetTutorialManager();

	StartGame();
}

void AEndlessGameMode::StartGame()
{
    tutorialManager->IsTutorialEnabled = false;
    // Add selected Widget to game
    if (UIWidget)
    {
        NecroWidget = CreateWidget<UUserWidget>(playerController, UIWidget);
        NecroWidget->AddToViewport();
    }

    if (UITrapPlacementWidget)
    {
        TrapPlacementWidget = CreateWidget<UUserWidget>(playerController, UITrapPlacementWidget);
        TrapPlacementWidget->AddToViewport();
        TrapPlacementWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (UIChronoWidget)
    {
        ChronoWidget = CreateWidget<UUserWidget>(playerController, UIChronoWidget);
        ChronoWidget->AddToViewport();
    }

    if (UISubGoalWidget)
    {
        SubGoalWidget = CreateWidget<UUserWidget>(playerController, UISubGoalWidget);
        SubGoalWidget->AddToViewport();
    }

    necroCharacter = Cast<ANecrognomiconCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (musicManager != nullptr) {
        musicManager->PlayMainMusicLevel(necroCharacter);
    }

    if (scoreManager != nullptr) {
        FTimerHandle MatchTimerHandle;
        if (!GetWorld()->GetTimerManager().IsTimerActive(MatchTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle, this, &AEndlessGameMode::AddMatchRow, 1.0f, true);
        }
    }

    if (scoreManager)
    {
        scoreManager->mode = "Survival";
    }

}

void AEndlessGameMode::AddMatchRow()
{
    FString TimerEvent = "Timer";
    this->AddMatchRow(TimerEvent);
}

void AEndlessGameMode::AddMatchRow(FString Event)
{
    scoreManager->AddMatchRow(Event, 99, necroCharacter->GetActorLocation(), 0.0f, false, GetDifficulty(), GetCurrentWeapn());
}

void AEndlessGameMode::SetVisibilityTrapPlacementWidget(bool visibility)
{
    if (TrapPlacementWidget)
    {
        if (visibility) {
            //NecroWidget->GetRootWidget()->SetVisibility(ESlateVisibility::Hidden);
            TrapPlacementWidget->SetVisibility(ESlateVisibility::Visible);
        }
        else {
            //NecroWidget->GetRootWidget()->SetVisibility(ESlateVisibility::Visible);
            TrapPlacementWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void AEndlessGameMode::LoadStartScreen()
{
    // Save CSV Game info
    scoreManager->SaveCSV();

    tutorialManager->ResetManager();

    FString levelName = "StartScreen_Level";
    FName currentLevel = FName(*levelName);
    UGameplayStatics::OpenLevel(this, currentLevel);
}

void AEndlessGameMode::RestartGame()
{
    if (CurrentWidget)
    {
        // Save CSV Game info
        scoreManager->SaveCSV();

        scoreManager->Reset();

        tutorialManager->ResetManager();

        playerController->inputModeGameOnly.SetConsumeCaptureMouseDown(false);
        playerController->SetInputMode(playerController->inputModeGameOnly);

        playerController->bShowMouseCursor = false;
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        UGameplayStatics::SetGamePaused(this, false);
        FString levelName = UGameplayStatics::GetCurrentLevelName(this);
        FName level = FName(*levelName);
        UGameplayStatics::OpenLevel(this, level);
    }
}

void AEndlessGameMode::CreateLoseWidget()
{
    if (UILoseWidget)
    {
        LoseWidget = CreateWidget<UUserWidget>(playerController, UILoseWidget);
        LoseWidget->AddToViewport();
        CurrentWidget = LoseWidget;

        LoseWidget->SetKeyboardFocus();
        playerController->bShowMouseCursor = true;

        UGameplayStatics::SetGamePaused(this, true);
    }
}

void AEndlessGameMode::createPauseMenu()
{
    if (CurrentWidget == PauseWidget || CurrentWidget == OptionsWidget || CurrentWidget == GlosaryWidget
        || !CurrentWidget)
    {
        if (UGameplayStatics::IsGamePaused(this)) {
            if ((UIPauseWidget != nullptr || UIOptionsWidget != nullptr || UIGlosaryWidget != nullptr)
                && CurrentWidget) {
                ResumeGame();
            }
        }
        else {
            if (UIPauseWidget != nullptr) {
                PauseWidget = CreateWidget<UUserWidget>(playerController, UIPauseWidget);
                PauseWidget->AddToViewport();
                CurrentWidget = PauseWidget;

                playerController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
                playerController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                playerController->inputModeGameAndUI.SetWidgetToFocus(PauseWidget->TakeWidget());
                playerController->SetInputMode(playerController->inputModeGameAndUI);

                PauseWidget->SetKeyboardFocus();
                playerController->bShowMouseCursor = true;

                UGameplayStatics::SetGamePaused(this, true);
                UE_LOG(LogTemp, Warning, TEXT("Activating pause widget"));

                if (musicManager != nullptr) {
                    musicManager->TurnOffGeneralVolume(necroCharacter);
                }
            }
        }
        if (UIPauseWidget == nullptr) {
            UE_LOG(LogTemp, Error, TEXT("pauseMenu es NULL"));
        }
    }
}

void AEndlessGameMode::OpenOptionsMenu()
{
    if (CurrentWidget)
    {
        //Remove the pause menu widget
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
        //Create the options widget
        OptionsWidget = CreateWidget<UUserWidget>(playerController, UIOptionsWidget);
        OptionsWidget->AddToViewport();
        CurrentWidget = OptionsWidget;

        UE_LOG(LogTemp, Warning, TEXT("Opening options menu"));

    }
}

void AEndlessGameMode::OpenGlosary()
{
    if (CurrentWidget)
    {
        //Remove the pause menu widget
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
        //Create the options widget
        GlosaryWidget = CreateWidget<UUserWidget>(playerController, UIGlosaryWidget);
        GlosaryWidget->AddToViewport();
        CurrentWidget = GlosaryWidget;

        UE_LOG(LogTemp, Warning, TEXT("Opening glosary"));

    }
}

void AEndlessGameMode::OpenBackPauseMenu()
{
    if (CurrentWidget)
    {
        //Remove the pause menu widget
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
        //Create the options widget
        PauseWidget = CreateWidget<UUserWidget>(playerController, UIPauseWidget);
        PauseWidget->AddToViewport();
        CurrentWidget = PauseWidget;

        UE_LOG(LogTemp, Warning, TEXT("Opening pause menu again"));

    }
}

void AEndlessGameMode::ResumeGame()
{
    if (CurrentWidget)
    {
        playerController->inputModeGameOnly.SetConsumeCaptureMouseDown(false);
        playerController->SetInputMode(playerController->inputModeGameOnly);

        playerController->bShowMouseCursor = false;
        //PauseWidget->RemoveFromParent();
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        UGameplayStatics::SetGamePaused(this, false);
        UE_LOG(LogTemp, Warning, TEXT("Deactivating pause widget"));

        if (musicManager != nullptr) {
            musicManager->TurnOnGeneralVolume();
        }
    }
}