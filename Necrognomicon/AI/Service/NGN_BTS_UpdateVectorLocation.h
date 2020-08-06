// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "NGN_BTS_UpdateVectorLocation.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTS_UpdateVectorLocation : public UBTService
{
	GENERATED_BODY()

private:
	USceneComponent* sceneComponent;

public:

	UNGN_BTS_UpdateVectorLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector componentKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector vectorKey;

};
