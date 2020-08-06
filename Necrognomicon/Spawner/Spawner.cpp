// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Pawn.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
}


/// <summary>
/// Spawns actors in random positions inside a radius.
/// </summary>
/// <param name="numberOfActors">The number of actors.</param>
/// <param name="radius">The radius.</param>
void ASpawner::SpawnActors(int32 numberOfActors, float radius)
{
	TArray<AActor*> SpawnedAI;
	FTransform spawnerTransform = GetTransform();

	FMath::RandInit((int32) (FDateTime::Now().GetTicks() % INT_MAX));

	for (int32 i = 0; i < numberOfActors; ++i)
	{
		FVector2D offset = FMath::RandPointInCircle(radius);
		FTransform spawnTransform = spawnerTransform;
		FVector location = FVector(
			spawnerTransform.GetLocation().X + offset.X,
			spawnerTransform.GetLocation().Y + offset.Y,
			spawnerTransform.GetLocation().Z);
		spawnTransform.SetLocation(location);
		GetWorld()->SpawnActor<AActor>(_subclassOfActor.Get(), spawnTransform);
	}
}


// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

