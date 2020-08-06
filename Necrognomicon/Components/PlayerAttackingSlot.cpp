// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackingSlot.h"

// Sets default values for this component's properties
APlayerAttackingSlot::APlayerAttackingSlot()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
}


// Called when the game starts
void APlayerAttackingSlot::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void APlayerAttackingSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ...
}

