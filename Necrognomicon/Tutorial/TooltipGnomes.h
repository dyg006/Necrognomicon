// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TooltipGnomes.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NECROGNOMICON_API UTooltipGnomes : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UTooltipGnomes();

	AActor* ParentActor;

public:
	UFUNCTION(BlueprintCallable, Category = "Tutorial")
		void ShowTooltip();

	UFUNCTION(BlueprintCallable, Category = "Tutorial")
		void HideTooltip();
	
};
