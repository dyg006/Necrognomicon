// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "Inventory/BPAObject.h"

// Sets default values for this component's properties
UFloorWeaponComponent::UFloorWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFloorWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	_currentItem = Cast<ABPAObject>(GetOwner());
	_rotation = FRotator(PitchValue, YawValue, RollValue);
	_rotatingMesh = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

}


// Called every frame
void UFloorWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PitchValue == 0.f && YawValue == 0.f && RollValue == 0.f || _currentItem->IsInInventory) {
		UActorComponent::SetComponentTickEnabled(false);
	}

	// on every frame change rotationg for a smooth rotating actor
	if (_rotatingMesh)
		_rotatingMesh->AddWorldRotation(_rotation);
}