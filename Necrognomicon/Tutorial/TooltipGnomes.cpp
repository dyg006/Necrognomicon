// Fill out your copyright notice in the Description page of Project Settings.


#include "TooltipGnomes.h"
#include "Kismet/KismetMathLibrary.h"

UTooltipGnomes::UTooltipGnomes() {
	ParentActor = GetOwner();
	SetBlendMode(EWidgetBlendMode::Transparent);
}

void UTooltipGnomes::ShowTooltip()
{
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector CameraLocation = CameraManager->GetCameraLocation();

	SetVisibility(true);
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(ParentActor->GetActorLocation(), CameraLocation);
	SetWorldRotation(Rotation);
}

void UTooltipGnomes::HideTooltip()
{
	SetVisibility(false);
}
