// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Set.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROGNOMICON_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Door")
	void ResetCloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float initialYawRightDoor;
	float initialYawLeftDoor;

	float currentYawRightDoor;
	float currentYawLeftDoor;

	UPROPERTY(BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* rightDoor;
	UPROPERTY(BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* leftDoor;

	UPROPERTY(EditAnywhere)
	float openAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float openVelocity = 0.8f;

	UPROPERTY(EditAnywhere)
	float closeVelocity = 2.f;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
