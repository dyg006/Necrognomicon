// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "NecrognomiconCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "EjectCharacter.generated.h"

UCLASS()
class NECROGNOMICON_API AEjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEjectCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/// <summary>
	/// Moves the rotation of the camera on the pitch axis.
	/// </summary>
	/// <param name="Val">The value.</param>
	void AddControllerPitchInput(float Val);

	/// <summary>
	/// Moves the rotation of the camera on the Yaw axis
	/// </summary>
	/// <param name="Val">The value.</param>
	void AddControllerYawInput(float Val);

	/// <summary>
	/// Possesses the main player.
	/// </summary>
	void Possess();

	/// <summary>
	/// The camera speed of the ejected actor
	/// </summary>
	float cameraMovement = 7.f;

	/// <summary>
	/// The rotation speed of the camera
	/// </summary>
	float rotationSpeed = 2.0f;

	/// <summary>
	/// Pauses the actors.
	/// </summary>
	void PauseActors();

	/// <summary>
	/// Resumes the actors.
	/// </summary>
	void ResumeActors();

	bool gamePaused = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// The base turn rate of the camera
	/// </summary>
	float BaseTurnRate;

	/// <summary>
	/// The base look up rate of the camera
	/// </summary>
	float BaseLookUpRate;

	/// <summary>
	/// The main character
	/// </summary>
	ANecrognomiconCharacter* necroCharacter;

	/// <summary>
	/// Check if the input has been activated
	/// </summary>
	bool inputIsActive = false;
};
