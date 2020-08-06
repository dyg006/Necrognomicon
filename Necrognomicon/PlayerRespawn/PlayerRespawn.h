// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PlayerRespawn.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API APlayerRespawn : public ATargetPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PlayerRespawn")
	int level = 1;
	
};
