// Fill out your copyright notice in the Description page of Project Settings.


#include "House.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "TimerManager.h"

// Sets default values
AHouse::AHouse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	healthComponent->alive = true;

	DamageType = UDamageType::StaticClass();
	DamagePerSec = 1.0f;
	PainInterval = 1.0f;

	//healthComponent->health = healthComponent->defaultHealth;
}

void AHouse::BeginPlay() 
{
	Super::BeginPlay();

	TArray<USceneComponent*> components;
	GetComponents<USceneComponent>(components);
	
	for (USceneComponent* slot : components) {
		if (slot->ComponentHasTag("Slot")) {
			attackingSlots.Add(slot);
			++numberOfSlots;
		}	
	}
}


// This is the actor damage handler.
float AHouse::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// DamageType example and properties
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	DamageTypeCDO->DamageImpulse;
	DamageTypeCDO->bCausedByWorld;

	// Health component
	if (ActualDamage != 0.f)
	{
		healthComponent->Damage(ActualDamage);
		this->UnderAttackEvent.Broadcast();
		
		// if dead destroy
		if (!healthComponent->alive) {
			//Destroy();
			ANecrognomiconGameMode* necroGameMode = Cast<ANecrognomiconGameMode>(GetWorld()->GetAuthGameMode());
			necroGameMode->GameOver();
		}
	}

	return ActualDamage;
}

void AHouse::Reset()
{
	healthComponent->health = healthComponent->defaultHealth;
	healthComponent->alive = true;
}

FVector AHouse::GetNextSlot() 
{
	int nextSlotIndex;

	if (numberOfSlots > 0) {
		nextSlotIndex = nextSlot;
		nextSlot = (nextSlot + 1) % numberOfSlots;
		return attackingSlots[nextSlotIndex]->GetComponentLocation();
	}
	
	return GetActorLocation();
	
}