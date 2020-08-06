// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SteeringBehaviour.generated.h"

UENUM(BlueprintType)
namespace EBTSteeringType
{
	enum Type
	{
		reset,      // This methods sets to zero the force to apply on the steeringManager owner
		update,     // This method is the one to call to update the owner's position
		
		seek,		// Force to move towards the target
		flee,		// Force to move from the target
		arrival,	// Same as flee but with acceptance radius
		pursuit,	// Similar to seek but with target movemen prediction
		evade,		// Similar to flee but with target movemen prediction

		wander,		// Creates random points according to manager configuration
		flock       // Apply the flock force to the manager owner
	};
}

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UBTTask_SteeringBehaviour : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY(Category = Steering, EditAnywhere)
	TEnumAsByte<EBTSteeringType::Type> steeringToApply;

public:
	/** starts this task, should return Succeeded, Failed or InProgress
	*  (use FinishLatentTask() when returning InProgress)
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
