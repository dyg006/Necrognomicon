// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_VisualPath.h"

// Sets default values
ANGN_VisualPath::ANGN_VisualPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANGN_VisualPath::BeginPlay()
{
	Super::BeginPlay();

	UpdatePath();

	SetVisible(false);
}

// Called every frame
void ANGN_VisualPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANGN_VisualPath::UpdatePath()
{
	for (int i = 0; i < _waypoints.Num() - 1; ++i)
	{
		_waypoints[i]->SetNextPoint(_waypoints[i + 1]);
	}
}

void ANGN_VisualPath::SetVisible(bool visible)
{
	for (auto& waypoint : _waypoints)
	{
		waypoint->SetActorHiddenInGame(!visible);
	}
}

#if WITH_EDITOR
void ANGN_VisualPath::PostEditChangeProperty(FPropertyChangedEvent& event)
{
	if (event.Property == nullptr) { return; }

	FName changedPropertyName = event.Property->GetFName();

	FName classPropertyName = GET_MEMBER_NAME_CHECKED(ANGN_VisualPath, _update);

	if (changedPropertyName.Compare(classPropertyName) == 0)
	{
		if (_update)
		{
			UpdatePath();
			_update = false;
		}
	}

	Super::PostEditChangeProperty(event);
}
#endif

