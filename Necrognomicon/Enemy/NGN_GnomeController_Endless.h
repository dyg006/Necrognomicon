// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "NavigationSystem.h"

#include "NGN_GnomeController_Endless.generated.h"

/**
 *
 */
UCLASS()
class NECROGNOMICON_API ANGN_GnomeController_Endless : public AAIController
{
	GENERATED_BODY()
	
public:

	FVector GetRandomPointNavigable(FVector origin, float radius);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;

	UBlackboardComponent* Blackboard;

	UNavigationSystemV1* NavSys;

	//UAIPerceptionComponent* AIPerception;

	void BeginPlay() override;
};
