// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (rightDoor == nullptr || leftDoor == nullptr) {
		return;
	}
	initialYawRightDoor = rightDoor->GetRelativeTransform().GetRotation().Rotator().Yaw;
	currentYawRightDoor = initialYawRightDoor;
	initialYawLeftDoor  = leftDoor->GetRelativeTransform().GetRotation().Rotator().Yaw;
	currentYawLeftDoor  = initialYawLeftDoor;
	openAngle += initialYawRightDoor;
	
}


void UOpenDoor::OpenDoor(float DeltaTime) {	
	if (rightDoor == nullptr || leftDoor == nullptr) {
		return;
	}
	currentYawRightDoor = FMath::Lerp(currentYawRightDoor, openAngle, DeltaTime * openVelocity);
	currentYawLeftDoor = FMath::Lerp(currentYawLeftDoor, -openAngle, DeltaTime * openVelocity);
	//currentYaw = FMath::FInterpTo(currentYaw, openAngle, DeltaTime, interpSpeed);
	FRotator rightDoorRotation = rightDoor->GetRelativeTransform().GetRotation().Rotator();
	rightDoorRotation.Yaw = currentYawRightDoor;

	FRotator leftDoorRotation = leftDoor->GetRelativeTransform().GetRotation().Rotator();
	leftDoorRotation.Yaw = currentYawLeftDoor;
	
	rightDoor->SetRelativeRotation(rightDoorRotation);
	leftDoor->SetRelativeRotation (leftDoorRotation);

	/*closeDoorSound = false;
	if (openAudio != nullptr) {
		if (!openAudio->IsPlaying() && !openDoorSound) {
			openAudio->Play();
			openDoorSound = true;
		}
	}*/
}

void UOpenDoor::CloseDoor(float DeltaTime) {

	if (rightDoor == nullptr || leftDoor == nullptr) {
		return;
	}
	currentYawRightDoor = FMath::Lerp(currentYawRightDoor, initialYawRightDoor, DeltaTime * openVelocity);
	currentYawLeftDoor = FMath::Lerp(currentYawLeftDoor, initialYawLeftDoor, DeltaTime * openVelocity);
	//currentYaw = FMath::FInterpTo(currentYaw, openAngle, DeltaTime, interpSpeed);
	FRotator rightDoorRotation = rightDoor->GetRelativeTransform().GetRotation().Rotator();
	rightDoorRotation.Yaw = currentYawRightDoor;

	FRotator leftDoorRotation = leftDoor->GetRelativeTransform().GetRotation().Rotator();
	leftDoorRotation.Yaw = currentYawLeftDoor;

	rightDoor->SetRelativeRotation(rightDoorRotation);
	leftDoor->SetRelativeRotation(leftDoorRotation);

	/*openDoorSound = false;
	if (openAudio != nullptr) {
		if (!openAudio->IsPlaying() && !closeDoorSound) {
			openAudio->Play();
			closeDoorSound = true;
		}
	}*/
}

void UOpenDoor::ResetCloseDoor() {

	if (rightDoor == nullptr || leftDoor == nullptr) {
		return;
	}

	currentYawRightDoor = initialYawRightDoor;
	currentYawLeftDoor = initialYawLeftDoor;

	FRotator rightDoorRotation = rightDoor->GetRelativeTransform().GetRotation().Rotator();
	rightDoorRotation.Yaw = currentYawRightDoor;

	FRotator leftDoorRotation = leftDoor->GetRelativeTransform().GetRotation().Rotator();
	leftDoorRotation.Yaw = currentYawLeftDoor;

	rightDoor->SetRelativeRotation(rightDoorRotation);
	leftDoor->SetRelativeRotation(leftDoorRotation);
}
