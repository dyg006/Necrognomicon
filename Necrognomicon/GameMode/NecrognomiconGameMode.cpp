// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecrognomiconGameMode.h"
#include "Components/SlotManagerComponent.h"
#include "NGN_GameInstance.h"
#include "Character/NecrognomiconCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/NecrognomiconController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Managers/NGN_ScoreManager.h"
#include "Managers/NGN_GnomeManager.h"
#include "Managers/VideoTutorialManager.h"

ANecrognomiconGameMode::ANecrognomiconGameMode()
{
    CurrentLevel = 1;
}

void ANecrognomiconGameMode::StartPlay()
{
	Super::StartPlay();
}

void ANecrognomiconGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* worldController = GetWorld()->GetFirstPlayerController();
    playerController = Cast<ANecrognomiconController>(worldController);

    musicManager    = Cast<UNGN_GameInstance>(GetGameInstance())->GetMusicManager();
    scoreManager    = Cast<UNGN_GameInstance>(GetGameInstance())->GetScoreManager();
    tutorialManager = Cast<UNGN_GameInstance>(GetGameInstance())->GetTutorialManager();
    gnomeManager    = Cast<UNGN_GameInstance>(GetWorld()->GetGameInstance())->GetGnomeManager();

    StartGame();    
}

void ANecrognomiconGameMode::StartGame()
{
    gameLost = false;

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

    necroCharacter = Cast<ANecrognomiconCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    spawnPlayerTransform = necroCharacter->GetTransform();

    if (musicManager != nullptr) {
        musicManager->PlayMainMusicLevel(necroCharacter);
    }

    if (scoreManager != nullptr) {
        FTimerHandle MatchTimerHandle;
        if (!GetWorld()->GetTimerManager().IsTimerActive(MatchTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(MatchTimerHandle, this, &ANecrognomiconGameMode::AddMatchRow, 1.0f, true);
        }
    }

    //Initialize the slot manager
    slotManager = necroCharacter->GetSlotManager();

    if (slotManager != nullptr ) {
        slotManager->CreatePlayerSlots();
    }

    if (scoreManager)
    {
        scoreManager->mode = "Adventure";
    }
}

void ANecrognomiconGameMode::AddMatchRow()
{
    FString TimerEvent = "Timer";
    this->AddMatchRow(TimerEvent);
}

void ANecrognomiconGameMode::AddMatchRow(FString Event)
{
    scoreManager->AddMatchRow(Event, CurrentLevel, necroCharacter->GetActorLocation(), GetHouseHealth(), GetHordeActive(), GetDifficulty(), GetCurrentWeapn());
}

void ANecrognomiconGameMode::CreateWinWidget()
{
    if (UIWinWidget)
    {
        scoreManager->UpdateGlobalCombos();
        if (victorySound != nullptr) {
            UFMODBlueprintStatics::PlayEvent2D(GetWorld(), victorySound, true);
        }

        necroCharacter->ResetCharacterParams();

        WinWidget = CreateWidget<UUserWidget>(playerController, UIWinWidget);
        WinWidget->AddToViewport();
        CurrentWidget = WinWidget;

        WinWidget->SetKeyboardFocus();
        GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

        UGameplayStatics::SetGamePaused(this, true);

        if (musicManager != nullptr) {
            musicManager->SetGameIntensityParam(gameFlagIntensity);
        }
    }
}

void ANecrognomiconGameMode::CreateLoseWidget()
{
    if (UILoseWidget)
    {
        scoreManager->UpdateGlobalCombos();
        if (gameOverSound != nullptr) {
            UFMODBlueprintStatics::PlayEvent2D(GetWorld(), gameOverSound, true);
        }

        necroCharacter->ResetCharacterParams();

        LoseWidget = CreateWidget<UUserWidget>(playerController, UILoseWidget);
        LoseWidget->AddToViewport();
        CurrentWidget = LoseWidget;

        LoseWidget->SetKeyboardFocus();
        playerController->bShowMouseCursor = true;
        
        UGameplayStatics::SetGamePaused(this, true);

        if (musicManager != nullptr) {
            musicManager->SetGameIntensityParam(gameFlagIntensity);
        }
    }
}

void ANecrognomiconGameMode::FinishGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), playerController,
        EQuitPreference::Quit, false);
}

void ANecrognomiconGameMode::RestartGame()
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

        gameLost = false;
    }
}

void ANecrognomiconGameMode::ResumeGame()
{
    if (CurrentWidget)
    {
        playerController->inputModeGameOnly.SetConsumeCaptureMouseDown(false);
        playerController->SetInputMode(playerController->inputModeGameOnly);

        playerController->bShowMouseCursor = false;
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        UGameplayStatics::SetGamePaused(this, false);
        UE_LOG(LogTemp, Warning, TEXT("Deactivating pause widget"));

        if (musicManager != nullptr) {
            musicManager->TurnOnGeneralVolume();
        }

        // esto no sirve: deprecaca
        //necroCharacter->canExecuteAction = true;
    }
}

void ANecrognomiconGameMode::RetryGame()
{
    if (CurrentWidget)
    {
        playerController->inputModeGameOnly.SetConsumeCaptureMouseDown(false);
        playerController->SetInputMode(playerController->inputModeGameOnly);

        playerController->bShowMouseCursor = false;
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        UGameplayStatics::SetGamePaused(this, false);
        ResetCurrentLevel();
        UE_LOG(LogTemp, Warning, TEXT("Deactivating game over widget"));

        gameLost = false;

        /*if (slotManager != nullptr) {
            slotManager->SetFreeAllSlots();
        }*/
    }
}

void ANecrognomiconGameMode::createPauseMenu()
{
    if (CurrentWidget == PauseWidget || CurrentWidget == OptionsWidget || CurrentWidget == GlosaryWidget
        || !CurrentWidget)
    {
        if (UGameplayStatics::IsGamePaused(this)) {
            if ((UIPauseWidget != nullptr || UIOptionsWidget != nullptr || UIGlosaryWidget != nullptr)
                && CurrentWidget) {
                UE_LOG(LogTemp, Warning, TEXT("Deactivating PAUSE"));
                necroCharacter->canExecuteAction = true;
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

                necroCharacter->canExecuteAction = false;

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

void ANecrognomiconGameMode::OpenOptionsMenu()
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

void ANecrognomiconGameMode::OpenGlosary()
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

void ANecrognomiconGameMode::OpenBackPauseMenu()
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

void ANecrognomiconGameMode::SetVisibilityTrapPlacementWidget(bool visibility)
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

void ANecrognomiconGameMode::RespawnPlayer() {
    FHitResult hitResult;

    if (spawnPoints.Num() > 0) {
        int indexWithCloserPoint = 0;
        float minDistance = 0.0f;
        for (int i = 0; i < spawnPoints.Num(); ++i) {
            if (i == 0) {
                minDistance = necroCharacter->GetDistanceTo(spawnPoints[0]);
            }
            else {
                if (necroCharacter->GetDistanceTo(spawnPoints[i]) < minDistance ) {
                    minDistance = necroCharacter->GetDistanceTo(spawnPoints[i]);
                    indexWithCloserPoint = i;
                }
            }
        }
        spawnPlayerTransform = spawnPoints[indexWithCloserPoint]->GetTransform();
    }

    // we cannot place the actor in this place because it is colliding with something
    if (necroCharacter->SetActorTransform(spawnPlayerTransform, false, &hitResult) == false) {
        if (hitResult.GetActor() != nullptr) {

            UE_LOG(LogTemp, Warning, TEXT("Cannot move object to location, blocked by %s"), *hitResult.GetActor()->GetName());
        }
    }
}

void ANecrognomiconGameMode::SetIsPlayerDead(bool playerDead)
{
    this->isPlayerDead = playerDead;
    
    if (playerDead) {
        if (slotManager != nullptr && gnomeManager != nullptr) {
            gnomeManager->ResetCurrentGnomes();
            slotManager->SetFreeAllSlots();
        }
    }
}

void ANecrognomiconGameMode::ResetLevel()
{
    UE_LOG(LogTemp, Warning, TEXT("Custom Reset"));

    // Reset ALL controllers first
    /*
    for (FConstControllerIterator Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
    {
        AController* Controller = Iterator->Get();
        APlayerController* PlayerController = Cast<APlayerController>(Controller);
        if (PlayerController)
        {
            PlayerController->ClientReset();
        }
        Controller->Reset();
    }
    */

    // Reset all actors (except controllers, the GameMode, and any other actors specified by ShouldReset())
    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor* A = *It;
        if (A && !A->IsPendingKill() && A != this && !A->IsA<AController>() && ShouldReset(A))
        {
            A->Reset();
        }
    }

    // Reset the GameMode
    Reset();

    // Notify the level script that the level has been reset
    /*
    ALevelScriptActor* LevelScript = GetWorld()->GetLevelScriptActor();
    if (LevelScript)
    {
        LevelScript->LevelReset();
    }
    */
}


void ANecrognomiconGameMode::LoadStartScreen()
{
    // Save CSV Game info
    scoreManager->SaveCSV();

    tutorialManager->ResetManager();
    
    FString levelName = "StartScreen_Level";
    FName currentLevel = FName(*levelName);
    UGameplayStatics::OpenLevel(this, currentLevel);
}

void ANecrognomiconGameMode::GameOver() {
    if (!gameLost) {
        // player died when house die not because player health
        scoreManager->PlayerDied();
        CreateLoseWidget();
        gameLost = true;
    }
}

void ANecrognomiconGameMode::IncreaseScore( int score ) {
    gnomesDestroyed += score;

    if (gnomesDestroyed >= numberOfTutorialGnomes && isTutorialDoorClosed && numberOfItemsPicked >= numberOfTutorialItems) {
        for (int i = 0; i < gates.Num(); ++i) {
            if (gates[i]->level == 0) {
                gates[i]->open = true;
            }
        }
       
        isTutorialDoorClosed = false;
        SetPathsVisibility(true);
    }
}

void ANecrognomiconGameMode::SetPlayerSlots() {
    if (slotManager != nullptr) {
        slotManager->RemovePlayerSlots();
        slotManager->CreatePlayerSlots();
    }
}