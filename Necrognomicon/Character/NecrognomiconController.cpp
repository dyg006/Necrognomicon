// Fill out your copyright notice in the Description page of Project Settings.


#include "NecrognomiconController.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "GameMode/NGN_GameInstance.h"
#include "GameMode/EndlessGameMode.h"

ANecrognomiconController::ANecrognomiconController() {
	// set our turn rates for input
	PrimaryActorTick.bCanEverTick = true;
	if (cinemaModeActive) {
		SetTickableWhenPaused(true);
		bShouldPerformFullTickWhenPaused = true;
	}
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void ANecrognomiconController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	gameInstance = Cast<UNGN_GameInstance>(GetGameInstance());

	_currentPawn = this->GetPawn();
	necroCharacter = Cast<ANecrognomiconCharacter>(_currentPawn);
	_necroMovement = necroCharacter->GetCharacterMovement();

	this->PlayerCameraManager->ViewPitchMax = necroCharacter->viewPitchMax;
	this->PlayerCameraManager->ViewPitchMin = necroCharacter->viewPitchMin;

	StopRunning();

	DefaultInputYawScale = InputYawScale;
	DefaultInputPitchScale = InputPitchScale;
	DefaultCapsuleRadius = necroCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	//UE_LOG(LogTemp, Warning, TEXT("Yaw, Pitch, Radius: %f %f %f"), DefaultInputYawScale, DefaultInputPitchScale, DefaultCapsuleRadius);
	SetMouseSensitivitySliderValue(mouseSensitivitySliderValue);
	SetGamepadSensitivitySliderValue(gamepadSensitivitySliderValue);
}

void ANecrognomiconController::Tick(float DeltaTime)
{
	//_currentPawn = this->GetPawn();

	Super::Tick(DeltaTime);

	// Check changes on Input Device
	if (shouldCheckInput || ticksNoCheckedInput >= 180) {
		//if (ticksNoCheckedInput >= 180)
			//UE_LOG(LogTemp, Warning, TEXT("Llamada por defecto"));

		if (gameInstance != nullptr)
			gameInstance->CheckInputDevice();

		shouldCheckInput = false;
		ticksNoCheckedInput = 0;
	}
	else {
		++ticksNoCheckedInput;
	}

	// Calculate Yaw Rotation scale based on current velocity
	if (isMowerWeaponSelected) 
	{
		float lastUpdateVelocity = necroCharacter->GetCharacterMovement()->GetLastUpdateVelocity().Size();
		InputYawScale = FMath::Clamp(lastUpdateVelocity, MowerStoppedRotationFactor, lastUpdateVelocity) / MowerRotationFactor;
		InputPitchScale = FMath::Clamp(lastUpdateVelocity, MowerStoppedRotationFactor, lastUpdateVelocity) / -MowerRotationFactor;

		if (isMowerWeaponActivated)
		{
			MoveForward(MowerSpeed);
		}
	}

}

void ANecrognomiconController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UE_LOG(LogTemp, Warning, TEXT("Init Input"));

	InputComponent->BindAxis("MoveForward", this, &ANecrognomiconController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ANecrognomiconController::MoveRight);

	InputComponent->BindAxis("TurnRate", this, &ANecrognomiconController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &ANecrognomiconController::LookUpAtRate);
	InputComponent->BindAxis("Turn", this, &ANecrognomiconController::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &ANecrognomiconController::AddControllerPitchInput);

	InputComponent->BindAction("MainAction", IE_Pressed, this, &ANecrognomiconController::MainAction);
	InputComponent->BindAction("MainAction", IE_Released, this, &ANecrognomiconController::MainActionStop);
	InputComponent->BindAction("ReloadAction", IE_Pressed, this, &ANecrognomiconController::Reload);
	InputComponent->BindAction("ReloadAction", IE_Released, this, &ANecrognomiconController::EndReload);
	InputComponent->BindAction("Run", IE_Pressed, this, &ANecrognomiconController::StartRunning);
	InputComponent->BindAction("Run", IE_Released, this, &ANecrognomiconController::StopRunning);
	InputComponent->BindAction("Pause", IE_Pressed, this, &ANecrognomiconController::Pause).bExecuteWhenPaused = true;
	
	
	InputComponent->BindAction("Jump", IE_Pressed, this, &ANecrognomiconController::StartJumping);
	InputComponent->BindAction("Jump", IE_Released, this, &ANecrognomiconController::StopJumping);

	InputComponent->BindAction("ChangeToRightAsset", IE_Released, this, &ANecrognomiconController::ChangeToRightAsset);
	InputComponent->BindAction("ChangeToLeftAsset", IE_Released, this, &ANecrognomiconController::ChangeToLeftAsset);
	InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ANecrognomiconController::MeleeAttack);
	InputComponent->BindAction("ChangeInventory", IE_Released, this, &ANecrognomiconController::ChangeInventory);
	InputComponent->BindAction("GetItem", IE_Pressed, this, &ANecrognomiconController::GetItem);
	InputComponent->BindAction("ZoomMap", IE_Pressed, this, &ANecrognomiconController::ZoomMap);

	InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ANecrognomiconController::SecondaryAction);
	InputComponent->BindAction("SecondaryAction", IE_Released, this, &ANecrognomiconController::SecondaryActionStop);

	if (cinemaModeActive) {
		SetupEjectInputComponent();
	}
}

void ANecrognomiconController::SetupEjectInputComponent() {
	InputComponent->BindAction("Eject/Possess", IE_Pressed, this, &ANecrognomiconController::Eject);
}

void ANecrognomiconController::MoveForward(float value)
{
	if (value != 0.0f && _currentPawn != nullptr)
	{
		// find out which way is forward
		//const FRotator Rotation = this->GetControlRotation();
		
		// Obtenemos la rotación de la cámara en vez de el personaje
		const FRotator Rotation = this->PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		_currentPawn->AddMovementInput(Direction, sprintIsActive ? value/abs(value) : value);

		// Actualizar el input device
		shouldCheckInput = true;
	}
}

void ANecrognomiconController::MoveRight(float value)
{
	if (value != 0.0f && _currentPawn != nullptr)
	{
		// find out which way is right
		//const FRotator Rotation = this->GetControlRotation();

		// Obtenemos la rotación de la cámara en vez de el personaje
		const FRotator Rotation = this->PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement in that direction
		if (!isMowerWeaponSelected)
		{
			_currentPawn->AddMovementInput(Direction, sprintIsActive ? value / abs(value) : value);
		}

		// Actualizar el input device
		shouldCheckInput = true;
	}
}

void ANecrognomiconController::TurnAtRate(float Rate)
{
	/*if (necroCharacter->cinemaModeActive) {
		return;
	}*/
	if (_currentPawn != nullptr) {
		// calculate delta for this frame from the rate information
		Rate = (FMath::Abs(Rate) < 0.5) ? Rate * 0.5 : Rate;
		_currentPawn->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

		// Actualizar el input device
		shouldCheckInput = true;
	}
}

void ANecrognomiconController::LookUpAtRate(float Rate)
{
	/*if (necroCharacter->cinemaModeActive) {
		return;
	}*/
	if (_currentPawn != nullptr) {

		if (canLookUp) {
			// calculate delta for this frame from the rate information
			Rate = (FMath::Abs(Rate) < 0.5) ? Rate * 0.5 : Rate;
			_currentPawn->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		}

		// Actualizar el input device
		shouldCheckInput = true;
	}
}

void ANecrognomiconController::AddControllerPitchInput(float Val)
{
	if (necroCharacter->cinemaModeActive) {
		return;
	}
	if (_currentPawn != nullptr) {
		_currentPawn->AddControllerPitchInput(Val * mouseSensitivity);
		

		// Actualizar el input device
		//shouldCheckInput = true;
	}
}

void ANecrognomiconController::AddControllerYawInput(float Val)
{
	if (necroCharacter->cinemaModeActive) {
		return;
	}
	if (_currentPawn != nullptr) {
		_currentPawn->AddControllerYawInput(Val * mouseSensitivity);

		// Actualizar el input device
		//shouldCheckInput = true;
	}
}

void ANecrognomiconController::StartRunning()
{
	AEndlessGameMode* endlessGameMode = Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode());
	if ( (!isMowerWeaponSelected || isMowerWeaponActivated) && !endlessGameMode) {
		UE_LOG(LogTemp, Warning, TEXT("StartRunning"));
		sprintIsActive = true;
		_necroMovement->MaxWalkSpeed = necroCharacter->_runnningSpeed;

		// Actualizar el input device
		shouldCheckInput = true;
	}
}

void ANecrognomiconController::StopRunning()
{
	UE_LOG(LogTemp, Warning, TEXT("StopRunning"));
	sprintIsActive = false;
	_necroMovement->MaxWalkSpeed = necroCharacter->_walkingSpeed;

	// Actualizar el input device
	//shouldCheckInput = true;
}

void ANecrognomiconController::CallStopRunning()
{
	StopRunning();
}

void ANecrognomiconController::MainAction()
{
	//TODO
	if (necroCharacter == NULL) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in MainAction"));
		return;
	}
	necroCharacter->MainAction();

	// Actualizar el input device
	shouldCheckInput = true;

	/*INNECESARIO AHORA QUE LAS PROPIAS ARMAS CONTROLAN LAS ANIMACIONES DE ATAQUE Y RELOAD*/
	/*
	if (weaponInventoryActive) {
		if (isMeleeWeaponSelected) {
			UE_LOG(LogTemp, Warning, TEXT("Main action with the weapon inventory active and melee weapon equipped"));
			necroCharacter->ActivateMeleeAnimation();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Main action with the weapon inventory active and range weapon equipped"));
			necroCharacter->ActivateFireAnimation();
		}	
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Main action with the trap inventory active"));
	}
	*/
}

void ANecrognomiconController::MainActionStop()
{
	//TODO
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in MainActionStop "));
		return;
	}
	necroCharacter->MainActionStop();

	// Actualizar el input device
	//shouldCheckInput = true;

}

void ANecrognomiconController::SecondaryAction()
{
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in MainAction"));
		return;
	}
	necroCharacter->SecondaryAction();

	// Actualizar el input device
	shouldCheckInput = true;
}

void ANecrognomiconController::SecondaryActionStop()
{
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in SecondaryActionStop"));
		return;
	}
	necroCharacter->SecondaryActionStop();

	// Actualizar el input device
	shouldCheckInput = true;
}

void ANecrognomiconController::Reload()
{
	//TODO
	UE_LOG(LogTemp, Warning, TEXT("Aim"));
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in Aim"));
		return;
	}
	necroCharacter->ReloadAction();
	/*INNECESARIO AHORA QUE LAS PROPIAS ARMAS CONTROLAN LAS ANIMACIONES DE ATAQUE Y RELOAD*/
	/*
	if (weaponInventoryActive && !isMeleeWeaponSelected) {
		UE_LOG(LogTemp, Warning, TEXT("Aiming with the weapon inventory active"));
		necroCharacter->ActivateReloadAnimation();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("You can not aim with the trap inventory selected. "));
	}
	*/

	// Actualizar el input device
	shouldCheckInput = true;
}


void ANecrognomiconController::EndReload()
{
	//TODO
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in MainActionStop "));
		return;
	}
	if (weaponInventoryActive) {
		UE_LOG(LogTemp, Warning, TEXT("Aim action stopped with the weapon inventory active"));
		necroCharacter->StopReloadAnimation();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Aim action stopped the trap inventory active"));
	}
}

void ANecrognomiconController::ChangeToRightAsset()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeToRightAsset"));
	//TSubclassOf<ANecrognomiconCharacter>* necroCharacter = Cast<TSubclassOf<ANecrognomiconCharacter>>(_currentPawn);
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in GetItem_Implementation "));
		return;
	}
	necroCharacter->ChangeToRightAsset();

	// Actualizar el input device
	shouldCheckInput = true;
}

void ANecrognomiconController::ChangeToLeftAsset()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeToLeftAsset"));
	//TSubclassOf<ANecrognomiconCharacter>* necroCharacter = Cast<TSubclassOf<ANecrognomiconCharacter>>(_currentPawn);
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in GetItem_Implementation "));
		return;
	}
	necroCharacter->ChangeToLeftAsset();

	// Actualizar el input device
	shouldCheckInput = true;
}

void ANecrognomiconController::ChangeInventory()
{
	//TODO
	UE_LOG(LogTemp, Warning, TEXT("ChangeInventory"));
	//TSubclassOf<ANecrognomiconCharacter>* necroCharacter = Cast<TSubclassOf<ANecrognomiconCharacter>>(_currentPawn);
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in GetItem_Implementation "));
		return;
	}
	necroCharacter->ChangeInventory();

	// Actualizar el input device
	shouldCheckInput = true;

}

void ANecrognomiconController::MeleeAttack()
{
	//TODO
	UE_LOG(LogTemp, Warning, TEXT("MeleeAttack"));
}

void ANecrognomiconController::GetItem_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Picked"));
	//TSubclassOf<ANecrognomiconCharacter>* necroCharacter = Cast<TSubclassOf<ANecrognomiconCharacter>>(_currentPawn);
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in GetItem_Implementation "));
		return;
	}
	necroCharacter->GetItem();

	// Actualizar el input device
	shouldCheckInput = true;
}

void ANecrognomiconController::ZoomMap_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Open map"));
	//TSubclassOf<ANecrognomiconCharacter>* necroCharacter = Cast<TSubclassOf<ANecrognomiconCharacter>>(_currentPawn);
	if (necroCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to NecrognomiconCharacter in GetItem_Implementation "));
		return;
	}
	isZoomMap = !isZoomMap;
	necroCharacter->ZoomMap();

	// Actualizar el input device
	shouldCheckInput = true;
}

void ANecrognomiconController::Pause()
{
	if (necroCharacter->cinemaModeActive) {
		return;
	}
	if (cinemaModeActive) {
		GetWorld()->bIsCameraMoveableWhenPaused = false;
	}
	// Actualizar el input device
	shouldCheckInput = true;

	UE_LOG(LogTemp, Warning, TEXT("Pause"));
	necroCharacter->StopOngoingActions();
	ANecrognomiconGameMode* gameMode = Cast<ANecrognomiconGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode) 
	{
		gameMode->createPauseMenu();
	}
	else {
		AEndlessGameMode* endlessGameMode = Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode());
		if (endlessGameMode)
		{
			endlessGameMode->createPauseMenu();
		}
	}
	
}

void ANecrognomiconController::StartJumping()
{
	if (!isMowerWeaponSelected && necroCharacter->canExecuteAction) {
		UE_LOG(LogTemp, Warning, TEXT("StartJumping"));
		ACharacter* character = Cast<ACharacter>(_currentPawn);
		
		if (character != nullptr) {
			character->Jump();

			// Actualizar el input device
			shouldCheckInput = true;
		}	
	}
}

void ANecrognomiconController::StopJumping()
{
	if (!isMowerWeaponSelected && necroCharacter->canExecuteAction) {
		UE_LOG(LogTemp, Warning, TEXT("StopJumping"));
		ACharacter* character = Cast<ACharacter>(_currentPawn);
		if (character != nullptr) {
			character->StopJumping();
		}
	}
}

void ANecrognomiconController::Eject()
{
	UE_LOG(LogTemp, Warning, TEXT("Eject"));
	
	if (necroCharacter != nullptr) {
		FVector ejectCharacterPosition = FVector(0, -500, 100);
		FViewTargetTransitionParams viewTargetTransition;
		viewTargetTransition.BlendExp = 0.7f;
		viewTargetTransition.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
		viewTargetTransition.BlendTime = 1.0f;
		viewTargetTransition.bLockOutgoing = true;
		if (ejectCharacter == nullptr) {
			ejectCharacter = GetWorld()->SpawnActor<AEjectCharacter>(AEjectCharacter::StaticClass(), necroCharacter->GetActorLocation() + ejectCharacterPosition, necroCharacter->GetActorRotation());
		}
		else {
			ejectCharacter->SetActorLocation(necroCharacter->GetActorLocation() + ejectCharacterPosition);
			ejectCharacter->SetActorRotation(necroCharacter->GetActorRotation());
			//Possess(Cast<APawn>(ejectCharacter));
			//AttachToPawn(ejectCharacter);
		}
		if (ejectCharacter != nullptr) {
			ejectCharacter->SetupPlayerInputComponent(InputComponent);
		}
		if (necroCharacter->gameMode != nullptr) {
			necroCharacter->gameMode->NecroWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (necroCharacter->endlessGameMode != nullptr) {
			necroCharacter->endlessGameMode->NecroWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		SetViewTarget(ejectCharacter, viewTargetTransition);
	}

	//SetCinematicMode(true, false, true, true, true);
}

void ANecrognomiconController::ActivateMower(float mRotation, float mSpeed, float mStoppedRotation)
{
	isMowerWeaponSelected = true;
	MowerRotationFactor = mRotation;
	MowerSpeed = mSpeed;
	MowerStoppedRotationFactor = mStoppedRotation;

	// Update input device
	shouldCheckInput = true;
}

void ANecrognomiconController::ModifyPlayerCollision()
{
	//Variables used to relocate mesh, camera etc
	FVector RelativeMeshLocation = necroCharacter->GetMesh()->GetRelativeTransform().GetLocation();
	FVector NewRelativeLocationMesh = RelativeMeshLocation + FVector(-MowerOffset, 0.f, 0.f);
	FVector RelativeCameraBoomLocation = necroCharacter->GetCameraBoom()->GetRelativeTransform().GetLocation();
	FVector NewRelativeCameraBoomLocation = RelativeCameraBoomLocation + FVector(-MowerOffset, 0.f, 0.f);

	//Debugging
	//UE_LOG(LogTemp, Warning, TEXT("Relative location: %f %f %f"), relativeLocationMesh.X, relativeLocationMesh.Y, relativeLocationMesh.Z);
	//UE_LOG(LogTemp, Warning, TEXT("New relative location: %f %f %f"), newRelativeLocationMesh.X, newRelativeLocationMesh.Y, newRelativeLocationMesh.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Offset, factor playercol: %f %f"), Offset, CollisionEnlargeFactor);

	// Enlarge character capsule collision & move mesh/cameraboom/actor according to offset
	necroCharacter->GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius * CollisionEnlargeFactor);
	necroCharacter->GetMesh()->SetRelativeLocation(NewRelativeLocationMesh);
	necroCharacter->GetCameraBoom()->SetRelativeLocation(NewRelativeCameraBoomLocation);
	necroCharacter->SetActorLocation(necroCharacter->GetActorLocation() + necroCharacter->GetActorForwardVector() * MowerOffset);

	PlayerCollisionModified = true;
}

void ANecrognomiconController::DeactivateMower()
{

	isMowerWeaponSelected = false;
	isMowerWeaponActivated = false;

	UE_LOG(LogTemp, Warning, TEXT("Yaw, Pitch, Radius: %f %f %f"), DefaultInputYawScale, DefaultInputPitchScale, DefaultCapsuleRadius);

	// Default value for Input Yaw Scale
	InputYawScale = DefaultInputYawScale;
	InputPitchScale = DefaultInputPitchScale;

	if (PlayerCollisionModified) {
		//Variables used to relocate mesh, camera etc
		FVector RelativeMeshLocation = necroCharacter->GetMesh()->GetRelativeTransform().GetLocation();
		FVector NewRelativeLocationMesh = RelativeMeshLocation + FVector(MowerOffset, 0.f, 0.f);
		FVector RelativeCameraBoomLocation = necroCharacter->GetCameraBoom()->GetRelativeTransform().GetLocation();
		FVector NewRelativeCameraBoomLocation = RelativeCameraBoomLocation + FVector(MowerOffset, 0.f, 0.f);

		// Restore character capsule collision & move mesh/cameraboom/actor according to offset
		if (necroCharacter)
		{
			necroCharacter->GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius);
			necroCharacter->GetMesh()->SetRelativeLocation(NewRelativeLocationMesh);
			necroCharacter->GetCameraBoom()->SetRelativeLocation(NewRelativeCameraBoomLocation);
			necroCharacter->SetActorLocation(necroCharacter->GetActorLocation() - necroCharacter->GetActorForwardVector() * MowerOffset);
		}

		PlayerCollisionModified = false;
	}

	// Actualizar el input device
	//shouldCheckInput = true;

}

void ANecrognomiconController::TurnOnMower()
{
	isMowerWeaponActivated = true;
	_necroMovement->MaxWalkSpeed = necroCharacter->_runnningSpeed;
}

void ANecrognomiconController::TurnOffMower()
{
	isMowerWeaponActivated = false;
	_necroMovement->MaxWalkSpeed = necroCharacter->_walkingSpeed;
}


void ANecrognomiconController::SetGamepadSensitivitySliderValue(const float sliderValue)
{
	gamepadSensitivitySliderValue = sliderValue;

	BaseTurnRate   = gamepadSensitivitySliderValue * 10;
	BaseLookUpRate = gamepadSensitivitySliderValue * 10;
}

void ANecrognomiconController::SetMouseSensitivitySliderValue(const float sliderValue)
{
	mouseSensitivitySliderValue = sliderValue;

	mouseSensitivity = sliderValue / 10;
}
