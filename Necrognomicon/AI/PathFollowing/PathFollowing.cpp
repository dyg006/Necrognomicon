// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFollowing.h"
#include "GameFramework/Pawn.h"
#include "Enemy/NGN_GnomeController.h"

// Sets default values for this component's properties
UPathFollowing::UPathFollowing()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPathFollowing::BeginPlay()
{
	Super::BeginPlay();

	_owner = GetOwner();

	if (_path != nullptr)
	{
		Cast<ANGN_GnomeController>(Cast<APawn>(_owner)->Controller)->SetPath(_path);
	}

	_offsetForNonUniformity = FMath::RandRange(-_radiusForNonUniformity, _radiusForNonUniformity);
	
}


// Called every frame
void UPathFollowing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPathFollowing::Reset()
{
	_currentNode = 0;
}

bool UPathFollowing::IsFinished() const
{
	if (_path == nullptr)
	{
		return true;
	}

	return _currentNode == _path->Length() - 1;
}

bool UPathFollowing::GetSteering(FVector &targetPosition, bool resetPosition)
{
	if ((_path == nullptr) || _path->IsEmpty())
	{
		return false;
	}
	/*
	if (resetPosition)
	{
		_currentNode = _path->GetClosestWaypoint(_owner->GetActorLocation());
	}
	*/

	// 1. Calculate the target to delegate to seek

	// Find the current position on the path
	float currentParam = _path->GetParam(_owner->GetActorLocation(), _currentNode);

	// Offset it
	float targetParam = currentParam + _pathOffset;

	// Get the target position
	FNGN_PathPosition position = _path->GetPosition(_currentNode, targetParam, _offsetForNonUniformity);
	targetPosition = position.position;

	// Update the current node
	_currentNode = position.waypoint;

	// 2. Delegate to seek
	return true;
}

void UPathFollowing::SetPath(ANGN_Path* path)
{
	_path = path;
	Reset();
}

