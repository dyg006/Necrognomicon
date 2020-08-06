// Fill out your copyright notice in the Description page of Project Settings.


#include "BPACoalTrap.h"
#include "TimerManager.h"
#include "GameFramework/DamageType.h"
#include "Enemy/NecrognomiconEnemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABPACoalTrap::ABPACoalTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsActive = false;

	BurningSoundComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("HeartbeatSound"));
	BurningSoundComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABPACoalTrap::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABPACoalTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsActive) {
		CausePain();
		if (BurningSoundComponent != nullptr) {
			if (!BurningSoundComponent->IsPlaying()) {
				BurningSoundComponent->Play();
			}
		}
	}
	else {
		if (BurningSoundComponent != nullptr) {
			if (BurningSoundComponent->IsPlaying()) {
				BurningSoundComponent->Stop();
			}
		}
	}
}

void ABPACoalTrap::action()
{
	if (!IsActive && OverGrid)
	{
		//Tiempo infinito al comentar esto
		//if (!GetWorldTimerManager().IsTimerActive(TimerHandle_DisabledTimer))
			//GetWorldTimerManager().SetTimer(TimerHandle_DisabledTimer, this, &ABPACoalTrap::DisabledTrap, ActiveTime, false);

		if (ActivationSound != nullptr) {
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSound, GetActorTransform(), true);
		}

		MakeItBurn();
	}
}

void ABPACoalTrap::Execute_Implementation()
{
	Super::Execute_Implementation();
	//if (OverGrid)
		action();
}

void ABPACoalTrap::Interaction_Implementation(){
	Super::Interaction_Implementation();
	if (ParticlesEmitter != nullptr)
	{
		ParticlesEmitter->DestroyComponent();
	}

	/*
	if (!IsActive) {
		Super::Interaction_Implementation();

		if (ParticlesEmitter != nullptr)
		{
			ParticlesEmitter->DestroyComponent();
		}
		
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're going to burn! The trap is active"));
		if (ErrorActionSound != nullptr) {
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ErrorActionSound, GetActorTransform(), true);
		}
	}
	*/
}

void ABPACoalTrap::MakeItBurn()
{
	this->IsActive = true;
	FVector emitterLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + EmitterAltitude);
	ParticlesEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticlesFX, emitterLocation);
	ParticlesEmitter->bRenderCustomDepth = EmitterCustomDepth;
	if (EmitterCustomDepth) {
		ParticlesEmitter->SetCustomDepthStencilValue(EmitterCustomStencilValue);
	}
}

void ABPACoalTrap::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (IsActive && DamageReceivers.Contains(OtherActor->GetClass()))
	{
		OverlapingEnemies.Add(OtherActor);
		if (BurningGnomeSound != nullptr) {
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), BurningGnomeSound, GetActorTransform(), true);
		}
	}
}

void ABPACoalTrap::CausePain()
{
	if (Damage > 0.f)
	{
		for (AActor* enemy : OverlapingEnemies) {
			UE_LOG(LogTemp, Error, TEXT("TakeDamage by trap: %f"), Damage);
			Cast<ANecrognomiconEnemy>(enemy)->ResidualDamageEvent.Broadcast(Damage, MotionModifier, CauseDamageTime, this);
		}
	}
}

void ABPACoalTrap::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (OverlapingEnemies.Contains(OtherActor))
	{
		OverlapingEnemies.Remove(OtherActor);
	}
}


void ABPACoalTrap::DisabledTrap()
{
	this->IsActive = false;
	ParticlesEmitter->DestroyComponent();
	GetWorldTimerManager().ClearTimer(TimerHandle_DisabledTimer);
}

