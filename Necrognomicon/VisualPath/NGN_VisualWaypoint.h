// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "CableComponent/Classes/CableComponent.h"
#include "NGN_VisualWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API ANGN_VisualWaypoint : public AActor, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	ANGN_VisualWaypoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCableComponent * Path = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* NextPoint = nullptr;

	// GetStatID and IsTickableInEditor are functions to debug in editor mode
	TStatId GetStatId() const override { return {}; }
	bool IsTickableInEditor() const override { return true; }

public:
	void SetNextPoint(ANGN_VisualWaypoint * point);

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;
};
