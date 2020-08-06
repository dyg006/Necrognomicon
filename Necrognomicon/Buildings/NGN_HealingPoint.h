// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/NecrognomiconCharacter.h"
#include "Components/HealthComponent.h"
#include "NGN_HealingPoint.generated.h"

UCLASS()
class NECROGNOMICON_API ANGN_HealingPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGN_HealingPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void NotifyActorEndOverlap(AActor* OtherActor) override;

	void Activate();

	void Deactivate();

	/// <summary>
	/// Is player overlapping
	/// </summary>
	bool _isPlayerOverlapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/// <summary>
	/// The healing speed in units per second
	/// </summary>
	float _healingSpeed;

	UHealthComponent* _healthComponent = nullptr;
	ANecrognomiconCharacter* _necroCharacter;

	FTimerHandle _timerHandle;

	bool _active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _endless = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/// <summary>
		/// Time between activations
		/// </summary>
		float _waitTime;

	/// <summary>
	/// Enemy skeletal mesh component
	/// </summary>
	UStaticMeshComponent* meshComponent = nullptr;

	/// <summary>
	/// Dynamic material to change its properties in runtime
	/// </summary>
	UMaterialInstanceDynamic* dynamicMaterial = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
