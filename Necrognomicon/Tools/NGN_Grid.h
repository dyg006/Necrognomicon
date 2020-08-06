// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGN_Grid.generated.h"

UCLASS()
class NECROGNOMICON_API ANGN_Grid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGN_Grid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grid")
	void SetFreeTileEvent(int32 row, int32 column, bool free);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grid")
	void IsFreeTileEvent(int32 row, int32 column, bool& free);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grid")
	void LocationToGridLocationEvent(FVector location, bool center, bool& valid, FVector2D& gridLocation, int32& row, int32& column);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grid")
	void TileToGridLocationEvent(int32 row, int32 column, bool& valid, float& locationX, float& locationY);
};
