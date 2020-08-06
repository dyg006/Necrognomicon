// Fill out your copyright notice in the Description page of Project Settings.


#include "BPABarrelTrap.h"
#include "TimerManager.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "Character/NecrognomiconCharacter.h"
#include "Character/NecrognomiconController.h"
#include "Components/BoxComponent.h"


// Sets default values
ABPABarrelTrap::ABPABarrelTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABPABarrelTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABPABarrelTrap::action()
{
	if (!shooted && OverGrid) {
		shooted = true;
		if (SkeletalMesh != nullptr) {
			if (ActivateAnimation != nullptr) {
				SkeletalMesh->PlayAnimation(ActivateAnimation, false);
			}
		}

		if (PreexplosionSound != nullptr) {
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), PreexplosionSound, GetActorTransform(), true);
		}

		ExplodeBarrel();
	}
}

void ABPABarrelTrap::ExplodeBarrel()
{
	GetWorldTimerManager().SetTimer(TimerHandle_BarrelDestruction, this, &ABPABarrelTrap::DestroyBarrel, DelayBeforeDestruction, false);
}


void ABPABarrelTrap::DestroyBarrel()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BarrelDestruction);
	ParticlesEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticlesFX, GetActorLocation());
	//GetWorldTimerManager().SetTimer(TimerHandle_ParticlesTimer, this, &ABPABarrelTrap::StopParticlesEffects, ExplosionTimeFX, false);

	if (ActivationSound != nullptr) {
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSound, GetActorTransform(), true);
	}

	// Mark tile as free, check if there is Grid assigned
	if (Grid) {
		Grid->SetFreeTileEvent(Row, Column, true);
	}

	//this->Destroy();
	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);

	if (Damage > 0.0f)
	{
		ANecrognomiconCharacter* ignorePlayer = Cast<ANecrognomiconCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageType, TArray<AActor*>(), ignorePlayer);
		TArray<AActor*> ignoreArray;
		ignoreArray.Add(ignorePlayer);
		//UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Damage, 200, GetActorLocation(), DamageRadius, DamageRadius + 1000, 
			//1.0, DamageType, ignoreArray, this, this->GetInstigatorController())
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageType, ignoreArray, this, this->GetInstigatorController(), true, ECollisionChannel::ECC_GameTraceChannel4);
	}
}

void ABPABarrelTrap::StopParticlesEffects()
{
	/*
	GetWorldTimerManager().ClearTimer(TimerHandle_ParticlesTimer);
	//Esto crashea Unreal con el nuevo vfx
	if (ParticlesEmitter)
	{
		ParticlesEmitter->DestroyComponent();
	}
	*/
}


// Called every frame
void ABPABarrelTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABPABarrelTrap::Reset()
{
	shooted = false;
	this->SetActorHiddenInGame(false);
	this->SetActorEnableCollision(true);
	
	if (Grid)
	{
		Grid->SetFreeTileEvent(Row, Column, false);
	}
	
	if (SkeletalMesh != nullptr) {
		if (ConstructionAnimation != nullptr) {
			SkeletalMesh->PlayAnimation(ConstructionAnimation, false);
		}
	}
}