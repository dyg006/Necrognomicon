// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MyDamageType.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UMyDamageType : public UDamageType
{
	GENERATED_BODY()

public:

	// Impact direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageType)
	FVector Direction;
};
