// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerAttackingSlot.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROGNOMICON_API APlayerAttackingSlot : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	APlayerAttackingSlot();

	//USceneComponent* RootComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isFree = true;
};
