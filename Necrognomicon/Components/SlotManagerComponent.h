// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlotManagerComponent.generated.h"

class UNGN_GnomeManager;
class ANecrognomiconCharacter;
class APlayerAttackingSlot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROGNOMICON_API USlotManagerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlotManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Creates the player slots.
	/// </summary>
	void CreatePlayerSlots();

	///// <summary>
	///// Creates a fixed number of player slots.
	///// </summary>
	//void CreateFixedPlayerSlots();

	/// <summary>
	/// Removes the player slots.
	/// </summary>
	void RemovePlayerSlots();

	/// <summary>
	/// Gets the next player slot position.
	/// </summary>
	/// <returns></returns>
	APlayerAttackingSlot *GetClosestSlot(const FVector& enemyPosition);

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Sets to free all slots.
	/// </summary>
	void SetFreeAllSlots();

private:
	/// <summary>
	/// The number of attacking slots
	/// </summary>
	int numberOfSlots;

	/// <summary>
	/// The player attacking slots
	/// </summary>
	TArray<APlayerAttackingSlot*> playerSlots;

	UNGN_GnomeManager* gnomeManager;

	ANecrognomiconCharacter* necroCharacter;

	/// <summary>
	/// The character radius
	/// </summary>
	float characterRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float _maxRadius = 200.f;

	int endlessModePlayerSlots = 10;
};
