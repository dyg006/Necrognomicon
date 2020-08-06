// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "NGN_BTT_RotateToFaceBBEntry.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTT_RotateToFaceBBEntry : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()
	
	UNGN_BTT_RotateToFaceBBEntry(const FObjectInitializer& ObjectInitializer);
};
