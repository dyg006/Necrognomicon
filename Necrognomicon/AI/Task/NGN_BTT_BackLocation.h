// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NGN_BTT_BackLocation.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTT_BackLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector ResultLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector ForwardLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
};
