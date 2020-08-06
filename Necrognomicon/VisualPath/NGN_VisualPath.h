// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGN_VisualWaypoint.h"
#include "NGN_VisualPath.generated.h"

UCLASS()
class NECROGNOMICON_API ANGN_VisualPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGN_VisualPath();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	TArray<ANGN_VisualWaypoint*> _waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	bool _update;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Level)
	int32 Level;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdatePath();

	UFUNCTION(BlueprintCallable)
	void SetVisible(bool visible);

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& event) override;
#endif


};
