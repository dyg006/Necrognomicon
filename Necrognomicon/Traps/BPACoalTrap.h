// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPATrap.h"
#include "BPACoalTrap.generated.h"


UCLASS()
class NECROGNOMICON_API ABPACoalTrap : public ABPATrap
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABPACoalTrap();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float ActiveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float CauseDamageTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float MotionModifier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		UParticleSystem* FireParticlesFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float EmitterAltitude = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		bool EmitterCustomDepth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		int EmitterCustomStencilValue;

	FTimerHandle TimerHandle_DisabledTimer;
	class UParticleSystemComponent* ParticlesEmitter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	/// <summary>
	/// The burning sound component of the trap
	/// </summary>
	UFMODAudioComponent* BurningSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	/// <summary>
	/// The burning sound when hits a gnome
	/// </summary>
	UFMODEvent* BurningGnomeSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual	void action() override;

	void Execute_Implementation();

	void Interaction_Implementation();

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

	void MakeItBurn();

	void CausePain();

	void DisabledTrap();

private:
	//bool IsActive;
	TArray<AActor*> OverlapingEnemies;
};
