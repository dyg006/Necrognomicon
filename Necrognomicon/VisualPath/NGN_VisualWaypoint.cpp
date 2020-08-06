// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_VisualWaypoint.h"

#include "Kismet/KismetMathLibrary.h"


ANGN_VisualWaypoint::ANGN_VisualWaypoint()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ANGN_VisualWaypoint::BeginPlay()
{
	Super::BeginPlay();

	Path = Cast<UCableComponent>(GetComponentByClass(UCableComponent::StaticClass()));
}

void ANGN_VisualWaypoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Path)
	{
		AActor* attached = Path->AttachEndTo.OtherActor;
		if (attached)
		{
			FRotator orientation = UKismetMathLibrary::FindLookAtRotation(Path->GetComponentLocation(), attached->GetActorLocation());
			Path->SetWorldRotation(orientation);
		}
	}
}

void ANGN_VisualWaypoint::SetNextPoint(ANGN_VisualWaypoint* point)
{
	if (Path == nullptr)
	{
		Path = Cast<UCableComponent>(GetComponentByClass(UCableComponent::StaticClass()));
	}
	NextPoint = point;
	Path->SetAttachEndTo(NextPoint, TEXT("Point"));
	
}
