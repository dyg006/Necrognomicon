// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "BPATrap.h"
#include "BPABearTrap.generated.h"

UCLASS()
class NECROGNOMICON_API ABPABearTrap : public ABPATrap
{

	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABPABearTrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float CoolDown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle for efficient management of OnTimerTick timer
	FTimerHandle TimerHandle_ActivatedTimer;

public:	

	void UpdatePlacingPositionCpp();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void action() override;
	void Interaction_Implementation();

	// Damage overlapping actors
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CausePain();

	// Damage sigle actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CausePainTo(class AActor* Other);

	UFUNCTION(BlueprintCallable, Category = "Timer")
		virtual void ResetCoolDown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	/// <summary>
	/// The reactivation sound of the trap
	/// </summary>
		UFMODEvent* ReactivationSound;

private:
	bool IsEnabled;

};
