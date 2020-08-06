// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HealthComponent.h"
#include "GameFramework/DamageType.h"
#include "House.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnderAttack);

struct AttackingSlot {
	TArray<USceneComponent*> slotComponent;
	TArray<bool> free;
};

UCLASS()
class NECROGNOMICON_API AHouse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHouse();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = House)
	class UStaticMeshComponent* BaseMesh;

	// Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = House)
	class UHealthComponent* healthComponent;

	// specific class for overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = House)
	TSubclassOf<class AActor> EnemyType;

	/** Damage done per second to actors in this volume when bPainCausing=true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = House)
	float DamagePerSec;

	/** Type of damage done */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = House)
	TSubclassOf<UDamageType> DamageType;

	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = House)
	float PainInterval;

	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = House)
		bool isTakingDamage = false;

	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FUnderAttack UnderAttackEvent;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void Reset() override;

	/// <summary>
	/// Returns the attacking slots.
	/// </summary>
	/// <returns></returns>
	FVector GetNextSlot();

protected:

	virtual void BeginPlay() override;

	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;
	
	/// <summary>
	/// The attacking slots
	/// </summary>
	TArray<USceneComponent*> attackingSlots;

	/// <summary>
	/// The number of attacking slots of the house
	/// </summary>
	int numberOfSlots;

	/// <summary>
	/// The next slot to return
	/// </summary>
	int nextSlot = 0;
};
