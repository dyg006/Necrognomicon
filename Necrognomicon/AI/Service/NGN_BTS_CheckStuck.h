// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "NGN_BTS_CheckStuck.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTS_CheckStuck : public UBTService
{
	GENERATED_BODY()
	
public:
	UNGN_BTS_CheckStuck();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StuckDistance;
};
