// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NGN_Path.h"
#include "PathFollowing.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROGNOMICON_API UPathFollowing : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathFollowing();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/// <summary>
	/// Holds the path to follow
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PathFollowing")
	ANGN_Path * _path = nullptr;

	/// <summary>
	/// Holds the distance along the path to generate the target
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFollowing")
	float _pathOffset = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFollowing")
	float _radiusForNonUniformity = 300;

	float _offsetForNonUniformity;

	/// <summary>
	/// Holds the last past node on the path
	/// </summary>
	int32 _currentNode;

	AActor* _owner = nullptr;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Reset();

	bool IsFinished() const;

	/// <summary>
	/// Gets the steering.
	/// </summary>
	/// <param name="targetPosition">The target position to follow.</param>
	/// <param name="resetPosition">if set to <c>true</c> recalculates the total position.</param>
	/// <returns>Returns if the position is calculated</returns>
	bool GetSteering(FVector& targetPosition, bool resetPosition);

	void SetPath(ANGN_Path* path);
		
};
