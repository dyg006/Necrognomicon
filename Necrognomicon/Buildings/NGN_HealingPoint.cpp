// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_HealingPoint.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ANGN_HealingPoint::ANGN_HealingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = !_endless;
}

// Called when the game starts or when spawned
void ANGN_HealingPoint::BeginPlay()
{
	Super::BeginPlay();

	_isPlayerOverlapping = false;

	_active = true;

	meshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	dynamicMaterial = UMaterialInstanceDynamic::Create(meshComponent->GetMaterial(0), nullptr);
	meshComponent->SetMaterial(0, dynamicMaterial);
	
}

// Called every frame
void ANGN_HealingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isPlayerOverlapping && _active)
	{
		_necroCharacter->UpdateHeartbeatSound();
		_healthComponent->Heal(_healingSpeed * DeltaTime);
		_necroCharacter->HealthStatusEvent.Broadcast(this->_healthComponent->health);
	}

}

void ANGN_HealingPoint::Activate()
{
	_active = true;

	dynamicMaterial->SetScalarParameterValue("Black", 0.0f);
}

void ANGN_HealingPoint::Deactivate()
{
	_active = false;

	dynamicMaterial->SetScalarParameterValue("Black", 0.7f);
}



void ANGN_HealingPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->ActorHasTag("Player"))
	{
		_isPlayerOverlapping = true;
		_healthComponent = Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass()));

		_necroCharacter = Cast<ANecrognomiconCharacter>(OtherActor);

		if (_endless) 
		{
			_healthComponent->Heal(_healingSpeed);
			_necroCharacter->UpdateHeartbeatSound();
			_necroCharacter->HealthStatusEvent.Broadcast(this->_healthComponent->health);
			Destroy();
		}

	}
}

void ANGN_HealingPoint::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor->ActorHasTag("Player"))
	{
		_isPlayerOverlapping = false;
		if (_active)
		{
			GetWorldTimerManager().SetTimer(_timerHandle, this, &ANGN_HealingPoint::Activate, _waitTime, false, -1.0f);
			Deactivate();
		}
	}
}



