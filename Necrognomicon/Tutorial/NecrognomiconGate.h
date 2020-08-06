// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NecrognomiconGate.generated.h"

UCLASS()
class NECROGNOMICON_API ANecrognomiconGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANecrognomiconGate();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Level")
		/// <summary>
		/// The corresponding level of the gate
		/// </summary>
		int level;

	UPROPERTY(BlueprintReadWrite)
		bool open = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
