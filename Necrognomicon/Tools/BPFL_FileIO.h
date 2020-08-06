// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_FileIO.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UBPFL_FileIO : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, Category = "File I/O")
		static bool FileExistsInProjectDir(FString FileName);

};
