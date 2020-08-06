// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "NGN_BTS_CheckAstrayed.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTS_CheckAstrayed : public UBTService
{
	GENERATED_BODY()
	
public:

	UNGN_BTS_CheckAstrayed();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector followPosition;
};
