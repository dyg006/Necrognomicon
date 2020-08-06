// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGN_Waypoint.h"
#include "Tickable.h"
#include "NGN_Path.generated.h"

/// <summary>
/// Represents a position in a path
/// </summary>
USTRUCT(BlueprintType)
struct NECROGNOMICON_API FNGN_PathPosition
{
	GENERATED_BODY()

	/// <summary>
	/// Node in the path corresponding to the position
	/// </summary
	UPROPERTY(EditAnywhere)
	int32 waypoint;

	/// <summary>
	/// Position in the path
	/// </summary>
	UPROPERTY(EditAnywhere)
	FVector position;

	FNGN_PathPosition(int32 waypoint, FVector position)
	{
		this->waypoint = waypoint;
		this->position = position;
	}

	FNGN_PathPosition()
	{
		waypoint = 0;
		position = FVector::ZeroVector;
	}
};

UCLASS()
class NECROGNOMICON_API ANGN_Path : public AActor, public FTickableGameObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGN_Path();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	TArray<ANGN_Waypoint*> waypoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// GetStatID and IsTickableInEditor are functions to debug in editor mode
	TStatId GetStatId() const override { return {}; }

	bool IsTickableInEditor() const override { return true; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path|Debug")
	bool _drawPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path|Debug")
	FColor _lineColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path|Debug")
	float _arrowSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path|Debug")
	float _lineThickness;

	/// <summary>
	/// Calculates the next waypoint in the path
	/// </summary>
	/// <param name="nodes"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	int32 GetNextIndex(const TArray<ANGN_Waypoint*> &nodes, int32 index) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsEmpty() const;

	int32 Length() const { return waypoints.Num(); }

	/// <summary>
	/// Gets the closest point of the path to the given position
	/// </summary>
	/// <param name="position">Position we want to calculate the closest point</param>
	/// <param name="lastNode">Last visited node of the path</param>
	/// <returns>A param is a position relative to the path and a node</returns>
	float GetParam(const FVector &position, int32 lastNode) const;

	/// <summary>
	/// Gets a world position in the path and updates the node if necessary
	/// </summary>
	/// <param name="waypoint">Node to which belongs the param</param>
	/// <param name="param">Param we are converting to a position</param>
	/// <param name="perpendicularOffset">The perpendicular offset.</param>
	/// <param name="outIsFinished">The out is finished.</param>
	/// <returns>
	/// Path position
	/// </returns>
	FNGN_PathPosition GetPosition(int32 waypoint, float param, float perpendicularOffset) const;

	/// <summary>
	/// Gets the index of the closest node to the position.
	/// </summary>
	/// <param name="position">The position.</param>
	/// <returns></returns>
	int32 GetClosestWaypoint(FVector position);

};


