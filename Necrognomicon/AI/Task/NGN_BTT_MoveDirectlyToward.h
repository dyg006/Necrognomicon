// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveDirectlyToward.h"
#include "UObject/ObjectMacros.h"
#include "NGN_BTT_MoveDirectlyToward.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTT_MoveDirectlyToward : public UBTTask_MoveDirectlyToward
{
	GENERATED_BODY()

		UNGN_BTT_MoveDirectlyToward(const FObjectInitializer& ObjectInitializer);
	
};
