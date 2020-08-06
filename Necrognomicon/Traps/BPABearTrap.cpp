// Fill out your copyright notice in the Description page of Project Settings.


#include "BPABearTrap.h"
#include "TimerManager.h"
#include "GameFramework/DamageType.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ABPABearTrap::ABPABearTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsEnabled = true;	// Value was "false" I need it for autoplacing in grid

}


void ABPABearTrap::UpdatePlacingPositionCpp() {
	Super::UpdatePlacingPositionCpp();
	IsEnabled = true;
}


// Called when the game starts or when spawned
void ABPABearTrap::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABPABearTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABPABearTrap::action()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_ActivatedTimer))
		GetWorldTimerManager().SetTimer(TimerHandle_ActivatedTimer, this, &ABPABearTrap::ResetCoolDown, CoolDown, true);
	if (IsEnabled)
	{
		if (SkeletalMesh != nullptr) {
			if (ActivateAnimation != nullptr) {
				SkeletalMesh->PlayAnimation(ActivateAnimation, false);
			}
			if (ActivationSound != nullptr && ReactivationSound != nullptr) {
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSound, GetActorTransform(), true);
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ReactivationSound, GetActorTransform(), true);
			}

		}
		CausePain();
	}
}

void ABPABearTrap::Interaction_Implementation()
{
	Super::Interaction_Implementation();

	/*
	if (!IsActive)
		
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Watch your hand! The trap is active"));
		if (ErrorActionSound != nullptr) {
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ErrorActionSound, GetActorTransform(), true);
		}
	}
	*/
}

void ABPABearTrap::CausePain()
{
	IsActive = true;
	IsEnabled = false;
	TSet<AActor*> TouchingActors;

	for (TSubclassOf<AActor> CurrentReceiver : DamageReceivers)
	{
		TSet<AActor*> Aux;
		GetOverlappingActors(Aux, CurrentReceiver);

		TouchingActors.Append(Aux);
	}

	for (AActor* const Receiver : TouchingActors)
	{
		CausePainTo(Receiver);
	}

}

void ABPABearTrap::CausePainTo(AActor* Other)
{
	if (Damage > 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("TakeDamage by trap: %f"), Damage);
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		FHitResult Hit;
		// TODO: Controlar la direccion en funcion de la normal de la trampa
		FVector ShotDirection(0.f, 0.f, 1.f);
		FPointDamageEvent DamageEvent = FPointDamageEvent(Damage, Hit, ShotDirection, DmgTypeClass);
		Other->TakeDamage(Damage, DamageEvent, this->GetInstigatorController(), this);
	}
}

void ABPABearTrap::ResetCoolDown()
{
	IsActive = false;
	IsEnabled = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_ActivatedTimer);
}

