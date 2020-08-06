// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "Classes/Animation/AnimInstance.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/HealthComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/TargetPoint.h"
#include "Inventory/BPAObject.h"
#include "NecrognomiconCharacter.generated.h"

// Forward declarations
class ANecrognomiconGameMode;

class AEndlessGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthStatusDelegate, float, currentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRespawnDelegate);

UCLASS(config=Game)
class ANecrognomiconCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	FRotator CameraRotation;
	FVector CameraLocation;

	/* Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* healthComponent;

	/** Heartbeat sound component of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class UFMODAudioComponent* HeartbeatSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class UFMODAudioComponent* errorAudioComponent;

	//ANecrognomiconController* NecroController;

	/* This will be deleted when all the logic from inventory is at C++ */
	//UActorComponent* playerInventory;

	/// <summary>
	/// Player skeletal mesh component
	/// </summary>
	USkeletalMeshComponent* playerMeshComponent;

	/// <summary>
	/// The main character animation
	/// </summary>
	UAnimInstance* playerAnimation;
	
	/// <summary>
	/// firing anim montage of the main character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* firingAnimMontage;

	/// <summary>
	/// melee attack montage animation of the main character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* meleeAttackMontage;

	/// <summary>
	/// reload attack montage animation of the main character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* reloadMontage;

	/// <summary>
	/// pickup object anim montage of the main character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* pickupItemAnimMontage;

	/// <summary>
	/// hurt anim montage of the main character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* hurtAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Character")
	class UForceFeedbackEffect* playerHurt;

public:
	ANecrognomiconCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	/// <summary>
	/// The view pitch maximum of the camera
	/// </summary>
	float viewPitchMax = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	/// <summary>
	/// The view pitch minimum of the camera
	/// </summary>
	float viewPitchMin = -10.0;

	UPROPERTY(BlueprintReadWrite, Category = Character)
	/// <summary>
	/// Check if attack is ready in order to activate the animation
	/// </summary>
	bool attackIsReady;

	/// <summary>
	/// Binds to delegate.
	/// </summary>
	/// <returns></returns>
	UFUNCTION() void BindToDelegate();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// Activates the fire animation.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ActivateFireAnimation();

	/// <summary>
	/// Activates the aim animation.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ActivateReloadAnimation();

	/// <summary>
	/// Activates the melee animation.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ActivateMeleeAnimation();

	/// <summary>
	/// Stops the fire animation.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Character")
	void stopFireAnimation();

	/// <summary>
	/// Stops the aim animation.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Character")
	void StopReloadAnimation();

	/// <summary>
	/// Activates the melee animation.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ActivatePickupAnimation();

	/// <summary>
	/// Activates the hurt animation.
	/// </summary>
	void ActivateHurtAnimation();

	/// <summary>
	/// Function called when the character dies
	/// </summary>
	void Die();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Character")
		void GetItem();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void ChangeInventory();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void ChangeToLeftAsset();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void ChangeToRightAsset();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void MainAction();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void SecondaryAction();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void SecondaryActionStop();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void SkipText();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void MainActionStop();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void ReloadAction();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void ZoomMap();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void StopRunning();

	//Forces all weapon logic to stop immediately
	//(used when pausing the game via button, tutorial, etc)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
	void StopWeaponLogic();

	//Forces ongoing character actions to stop immediately (running & releasing horde)
	//(used when pausing the game via button, tutorial, etc)
	UFUNCTION(BlueprintCallable, Category = "Caharacter")
	void StopOngoingActions();

	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FHealthStatusDelegate HealthStatusEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FRespawnDelegate SpawnEvent;

	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

	/// <summary>
	/// Variable used in order to set the time we want the player to be invincible after the spawn
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character")
	float invincibleTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float secondsToExitCombat = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float healthRecoveryPerSecond = 1.f;

	/// <summary>
	/// Variable used in order to set the time we want the player to be invincible after the spawn
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character")
	float timeBeforeRespawn = 3.0f;

	/// <summary>
	/// Check if the player is in invincible state
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool isPlayerInvulnerable = false;

	/// <summary>
	/// Check if the player is dead
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool isPlayerDead = false;

	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	bool canExecuteAction = true;

	void ReceiveDamage(float Damage);

	void PainTimer();

	void InflictDamage(AActor* Other);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	/// <summary>
	/// Function called in order to initialize some values after the constructor finishes.
	/// </summary>
	void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	ANecrognomiconGameMode* gameMode;

	AEndlessGameMode* endlessGameMode;

	//UPROPERTY(EditAnywhere, Category = "Character")
	//float runningThreshold;

	UPROPERTY(EditAnywhere, Category = "Character")
	float _runnningSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Character")
	float _walkingSpeed = 600.0f;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory")
	void AnadirAInventario(FBPEObject contenido, int32 cantidad, int32 municion, bool& anadido);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Invetory")
	void ActualizarInventario();

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	class ABPAObject* pickableItem;
	float previousDistance = 0.f;

	/// <summary>
	/// Updates the heartbeat sound.
	/// </summary>
	void UpdateHeartbeatSound();

	/// <summary>
	/// Resets the character parameters.
	/// </summary>
	void ResetCharacterParams();

	/// <summary>
	/// Enables the collisions of the player.
	/// </summary>
	void RespawnPlayer();

	virtual void Reset() override;

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Gets the slot manager.
	/// </summary>
	class USlotManagerComponent* GetSlotManager() const;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		UFMODEvent* hurtSound;

	/// <summary>
	/// Check if the cinema mode is active
	/// </summary>
	bool cinemaModeActive = false;

	/// <summary>
	/// Plays the error sound when trying to place a trap where there is no grid.
	/// </summary>
	void PlayErrorSound();

protected:
	float ReceiveDamageAmount;

	/// <summary>
	/// check if the time of being invincible have passed
	/// </summary>
	float timeBeingInvulnerable = 0.0f;
	
	/// <summary>
	/// The time waiting for respawn
	/// </summary>
	float timeWaitingForRespawn = 0.0f;

	/// <summary>
	/// The collision of the character is enabled
	/// </summary>
	bool collisionEnabled = true;

	/// <summary>
	/// Disables the collisions of the player when he dies.
	/// </summary>
	void DisableCollisionsAndInput();

	/// <summary>
	/// Enables the collisions of the player.
	/// </summary>
	void EnableCollisionsAndInput();

	void CheckPickableObject();

	/// <summary>
	/// The time out of action
	/// </summary>
	float timeOutOfAction = 0.f;

	/// <summary>
	/// Checks if can restore health and if so do it.
	/// </summary>
	/// <param name="DeltaSeconds">The delta seconds.</param>
	void CheckIfCanRestoreHealth(float DeltaSeconds);

	float timeLasts = 0.f;

	/// <summary>
	/// Dynamic material to change its properties in runtime
	/// </summary>
	UMaterialInstanceDynamic* dynamicMaterial = nullptr;

	/// <summary>
	/// Dynamic material to change its properties in runtime
	/// </summary>
	UMaterialInstanceDynamic* secondaryDynamicMaterial = nullptr;

private:

	UFMODAudioComponent* hurtAudioComponent;

	/// <summary>
	/// Checks the player death.
	/// </summary>
	/// <param name="DeltaTime">The delta time.</param>
	void CheckPlayerDeath(float DeltaTime);

	/// <summary>
	/// Checks the camera collision.
	/// </summary>
	void CheckCameraCollision(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Camera")
	/// <summary>
	/// The classes ignored by camera colision
	/// </summary>
	TArray<UClass*> classesIgnoredByCamera;


	UPROPERTY(EditAnywhere, Category = "Camera")
	/// <summary>
	/// The meshes ignored by camera colision
	/// </summary>
	TArray<UStaticMesh*> staticMeshesIgnoredByCamera;

	UPROPERTY(EditAnywhere, Category = "Animation")
	/// <summary>
	/// The classes that triggers the hurt animation
	/// </summary>
	TArray<UClass*> classesTriggersHurtAnim;

	/// <summary>
	/// The camera created in order to check if the player has nothing behind
	/// </summary>
	UCameraComponent* ghostCamera = nullptr;

	/// <summary>
	/// Check if the ghost camera is created
	/// </summary>
	bool ghostCameraCreated = false;

	float cameraChangeLasts = 0.0f;
	float delayBeforeChangingTheCamera = 1.0f;

	class USlotManagerComponent* slotManager;

	/// <summary>
	/// The material change red delay
	/// </summary>
	float materialChangeRedDelay = 0.1;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The blinking delay
	/// </summary>
	float blinkingDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The blinking delay
	/// </summary>
	float resetBlinkingDelay = 0.08f;

	float blinkingTimer;

	/// <summary>
	/// The timer delay for material change to red
	/// </summary>
	FTimerHandle TimerHandle_MaterialChangeRed;

	/// <summary>
	/// The timer delay for material blinking
	/// </summary>
	FTimerHandle TimerHandle_Blinking;

	/// <summary>
	/// Changes the red material of the character to the default one.
	/// </summary>
	void ChangeRedMaterial();

	/// <summary>
	/// Starts the blinking.
	/// </summary>
	void StartBlinking();

	/// <summary>
	/// Resets the blinking.
	/// </summary>
	void ResetBlinking();

	/// <summary>
	/// Stops the blinking.
	/// </summary>
	void StopBlinking();
};
