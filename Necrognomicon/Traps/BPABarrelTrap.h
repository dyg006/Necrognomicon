// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPATrap.h"
#include "BPABarrelTrap.generated.h"


UCLASS()
class NECROGNOMICON_API ABPABarrelTrap : public ABPATrap
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABPABarrelTrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float ExplosionTimeFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		/// <summary>
		/// The delay before barrel destruction
		/// </summary>
		float DelayBeforeDestruction = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		UParticleSystem* ExplosionParticlesFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
		/// <summary>
		/// The reactivation sound of the trap
		/// </summary>
		UFMODEvent* PreexplosionSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_ParticlesTimer;
	class UParticleSystemComponent* ParticlesEmitter= nullptr;

	/// <summary>
	/// The timer delay for the barrel destruction
	/// </summary>
	FTimerHandle TimerHandle_BarrelDestruction;

	/// <summary>
	/// Check if the barrel have been already shooted
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool shooted = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void action() override;

	// Custom event
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void ExplodeBarrel();

	void StopParticlesEffects();

	/// <summary>
	/// Destroys the barrel object.
	/// </summary>
	void DestroyBarrel();

	virtual void Reset() override;

};
