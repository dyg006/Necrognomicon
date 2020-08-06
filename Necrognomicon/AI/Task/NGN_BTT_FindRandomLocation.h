// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NGN_BTT_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTT_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Executes the task.
	/// </summary>
	/// <param name="OwnerComp">The owner comp.</param>
	/// <param name="NodeMemory">The node memory.</param>
	/// <returns>Returns Succeded, Failed or In Progress</returns>
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector WanderRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector OriginLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector ResultLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Static;
};
