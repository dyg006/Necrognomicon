// Fill out your copyright notice in the Description page of Project Settings.

#include "NGN_Path.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANGN_Path::ANGN_Path()
    : _drawPath(true),
	_lineColor(FColor::Red),
	_arrowSize(200),
	_lineThickness(3)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANGN_Path::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANGN_Path::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if ENABLE_DRAW_DEBUG && WITH_EDITOR
	if(_drawPath)
	{
        for (int i = 0; i < waypoints.Num(); ++i)
        {
        	if (waypoints[i] != nullptr && waypoints[GetNextIndex(waypoints, i)] != nullptr)
            {
	            FVector start = waypoints[i]->GetActorLocation();
	            FVector end = waypoints[GetNextIndex(waypoints, i)]->GetActorLocation();

	            DrawDebugDirectionalArrow(GetWorld(), start, end,
	                    100, _lineColor, false, -1, 0, _lineThickness);
        	}
            
        }
	}
#endif


}

bool ANGN_Path::IsEmpty() const
{
    return waypoints.Num() == 0;
}


int32 ANGN_Path::GetNextIndex(const TArray<ANGN_Waypoint*> &nodes, int32 index) const
{
    int32 nextIndex = FMath::Min(index + 1, nodes.Num() - 1);

    return nextIndex;
}


float ANGN_Path::GetParam(const FVector &position, int32 lastNode) const
{
    FVector a = waypoints[lastNode]->GetActorLocation();
    FVector b = waypoints[GetNextIndex(waypoints, lastNode)]->GetActorLocation();

    // Vector to project the position in the segment
    FVector p = position - a;

    // Segment corresponding to the node
    FVector q = b - a;

    if (q.Size() == 0)
    {
        return 0;
    }    

    // Using the magnitude of the projection we get the param 
    FVector projection = p.ProjectOnTo(q);

    // We check if the projection has the same direction of the path
    // (1 same direction, -1 opposite)
    float direction = (FVector::DotProduct(projection, q) > 0 ? 1 : -1);

    return projection.Size() * direction;
}

            


FNGN_PathPosition ANGN_Path::GetPosition(int32 waypoint, float param, float perpendicularOffset) const
{
    FVector a = waypoints[waypoint]->GetActorLocation();
    FVector b = waypoints[GetNextIndex(waypoints, waypoint)]->GetActorLocation();
    FVector segment = b - a;
    float segmentLength = segment.Size();

    // If the param is beyond the next node we have to recalculate
    if (param >= segmentLength)
    {
        // We update the node and the rest of the param
        waypoint = GetNextIndex(waypoints, waypoint);
        param -= segmentLength;
        // We recalculate the segment
        a = waypoints[waypoint]->GetActorLocation();
        b = waypoints[GetNextIndex(waypoints, waypoint)]->GetActorLocation();
        segment = b - a;
    }

    // Transform the param into a position inside the path
    segment.Normalize();
    FVector normal = FVector::CrossProduct(segment, FVector::UpVector);
    segment *= param;
    segment += a;
    segment += normal * perpendicularOffset;

    return FNGN_PathPosition(waypoint, segment);
}

int32 ANGN_Path::GetClosestWaypoint(FVector position)
{
    int32 waypoint = 0;
    float minDistance = TNumericLimits<float>::Max();

	for (int i = 0; i < waypoints.Num() - 1; ++i)
	{
        FVector waypointPos = waypoints[i]->GetActorLocation();
        float distance = FVector::Distance(position, waypointPos);
		if (distance < minDistance)
		{
            waypoint = i;
            minDistance = distance;
		}
	}

    return waypoint;
}


