// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoriableInterface.generated.h"

UINTERFACE(BlueprintType)
class NECROGNOMICON_API UInventoriableInterface : public UInterface
{
	GENERATED_BODY()

};

class NECROGNOMICON_API IInventoriableInterface
{
	GENERATED_BODY()
public:
	// Select item in the inventory
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void Select();
	
	// Execute action
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void Execute();

	// Pick up the object
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void Interaction();
};