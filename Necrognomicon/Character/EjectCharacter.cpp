// Fill out your copyright notice in the Description page of Project Settings.

#include "EjectCharacter.h"
#include "Camera/CameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "GameMode/EndlessGameMode.h"


// Sets default values
AEjectCharacter::AEjectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bTickEvenWhenPaused = true;
	SetTickableWhenPaused(true);
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

// Called when the game starts or when spawned
void AEjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	necroCharacter = Cast<ANecrognomiconCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AEjectCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	//check(InputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Init Eject Input"));
	
	if (!inputIsActive) {
		InputComponent->BindAxis("MoveForwardCamera", this, &AEjectCharacter::MoveForward).bExecuteWhenPaused = true;
		InputComponent->BindAxis("MoveRightCamera", this, &AEjectCharacter::MoveRight).bExecuteWhenPaused = true;

		InputComponent->BindAxis("TurnCamera", this, &AEjectCharacter::AddControllerYawInput).bExecuteWhenPaused = true;
		InputComponent->BindAxis("LookUpCamera", this, &AEjectCharacter::AddControllerPitchInput).bExecuteWhenPaused = true;
		InputComponent->BindAction("Pause", IE_Pressed, this, &AEjectCharacter::PauseActors).bExecuteWhenPaused = true;
		inputIsActive = true;
	}

	// Used only for Cinematic Mode (ordered by marketing)
	InputComponent->BindAction("Eject/Possess", IE_Pressed, this, &AEjectCharacter::Possess).bExecuteWhenPaused = true;
	
	PauseActors();
	
}


void AEjectCharacter::MoveForward(float value)
{
	
	if (value != 0.0f && this != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE FORWARRRRD"));
		SetActorLocation(GetActorLocation() + GetActorUpVector() * value * cameraMovement); // FVector(0, 0, cameraMovement * value));
	}
}

void AEjectCharacter::MoveRight(float value)
{
	
	if (value != 0.0f && this != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE righttttttttttttttt"));

		SetActorLocation(GetActorLocation() + GetActorForwardVector() * value * cameraMovement);
	}
}

void AEjectCharacter::AddControllerPitchInput(float Val)
{
	SetActorRotation(GetActorRotation() + FRotator(-2 * Val, 0, 0));	
}

void AEjectCharacter::AddControllerYawInput(float Val)
{
	SetActorRotation(GetActorRotation() + FRotator(0, 2 * Val, 0));
}

void AEjectCharacter::Possess()
{
	UE_LOG(LogTemp, Warning, TEXT("Possess"));
	if (necroCharacter != nullptr) {
		FInputActionBinding inputAction;
		FViewTargetTransitionParams viewTargetTransition;
		viewTargetTransition.BlendExp = 0.7f;
		viewTargetTransition.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
		viewTargetTransition.BlendTime = 1.0f;
		viewTargetTransition.bLockOutgoing = true;

		if (necroCharacter->gameMode != nullptr) {
			necroCharacter->gameMode->NecroWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else if (necroCharacter->endlessGameMode != nullptr){
			necroCharacter->endlessGameMode->NecroWidget->SetVisibility(ESlateVisibility::Visible);
		}		
		GetWorld()->GetFirstPlayerController()->SetViewTarget(necroCharacter, viewTargetTransition);
		Cast<ANecrognomiconController>(GetWorld()->GetFirstPlayerController())->SetupEjectInputComponent();
		ResumeActors();
		if (necroCharacter != nullptr) {
			necroCharacter->cinemaModeActive = false;
		}
	}
}

// Called every frame
void AEjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEjectCharacter::PauseActors() 
{
	if (!UGameplayStatics::IsGamePaused(GetWorld())) {
		GetWorld()->bIsCameraMoveableWhenPaused = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);
		//CustomTimeDilation *= 10000.f;
		if (necroCharacter != nullptr) {
			necroCharacter->cinemaModeActive = true;
		}
	}
	else {
		ResumeActors();
	}

	/*TSubclassOf<AActor> classToFind;
	classToFind = AActor::StaticClass();
	TArray<AActor*> actorsToPause;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, actorsToPause);
	for (AActor* actor : actorsToPause) {
		if (actor != this && actor != GetWorld()->GetFirstPlayerController()) {
			actor->CustomTimeDilation = 0.f;
		}
	}*/
	
}

void AEjectCharacter::ResumeActors()
{
	/*TSubclassOf<AActor> classToFind;
	classToFind = AActor::StaticClass();
	TArray<AActor*> actorsToPause;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, actorsToPause);
	for (AActor* actor : actorsToPause) {
		actor->CustomTimeDilation = 1.f;		
	}*/
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}