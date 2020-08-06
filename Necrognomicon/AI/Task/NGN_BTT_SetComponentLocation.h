// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NGN_BTT_SetComponentLocation.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTT_SetComponentLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
	/*UNGN_BTT_SetComponentLocation(const FObjectInitializer& ObjectInitializer);*/

private:
	USceneComponent* sceneComponent;

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector componentKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector vectorKey;
};
