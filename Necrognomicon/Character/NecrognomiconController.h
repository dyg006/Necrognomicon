// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "EjectCharacter.h"
#include "NecrognomiconCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
//#include "GameMode/NGN_GameInstance.h"
//#include "GameFramework/Character.h"
#include "NecrognomiconController.generated.h"


class UNGN_GameInstance;

/**
 * 
 */
UCLASS()
class ANecrognomiconController : public APlayerController
{
	GENERATED_BODY()

public:
	ANecrognomiconController();

	/// <summary>
	/// The current pawn that player is controlling
	/// </summary>
	APawn* _currentPawn;

	/// <summary>
	/// The input mode used by the controller
	/// </summary>
	FInputModeGameOnly inputModeGameOnly;

	/// <summary>
	/// The input mode used by the controller
	/// </summary>
	FInputModeGameAndUI inputModeGameAndUI;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, Category = Camera) 
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, Category = Camera)
		float BaseLookUpRate;

	bool canLookUp = true;

	/// <summary>
	/// Check if the weapon inventory is active or not
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = inventory)
	bool weaponInventoryActive = true;

	/// <summary>
	/// Check if a melee weapon is selected or not
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = MeleeWeapon)
	bool isMeleeWeaponSelected = true;

	UPROPERTY(BlueprintReadWrite, Category = MeleeWeapon)
	bool isMowerWeaponSelected = false;

	UPROPERTY(BlueprintReadWrite, Category = MeleeWeapon)
	bool isMowerWeaponActivated = false;

	void SetupEjectInputComponent();

	UFUNCTION(BlueprintCallable)
		float GetGamepadSensitivitySliderValue() const { return gamepadSensitivitySliderValue; }
	UFUNCTION(BlueprintCallable)
		float GetMouseSensitivitySliderValue() const { return mouseSensitivitySliderValue; }

	UFUNCTION(BlueprintCallable)
		void SetGamepadSensitivitySliderValue(const float sliderValue);
	UFUNCTION(BlueprintCallable)
		void SetMouseSensitivitySliderValue(const float sliderValue);

	void CallStopRunning();

private:
	float DefaultInputYawScale;
	float DefaultInputPitchScale;
	float DefaultCapsuleRadius;
	float MowerRotationFactor;
	float MowerSpeed;
	float MowerStoppedRotationFactor;
	
	bool PlayerCollisionModified = false; // Whether or not the player's capsule collision is currently modified due to the mower
	float MowerOffset = 60.f; //Units to move the actor and components when equipping/unequipping mower to better fit collision
	float CollisionEnlargeFactor = 1.8f; // Factor by which capsule radius is multiplied when using the mower

	/*Changed in options menu*/
	UPROPERTY(EditAnywhere)
	float gamepadSensitivitySliderValue = 5.f;
	UPROPERTY(EditAnywhere)
	float mouseSensitivitySliderValue = 5.f;

	float mouseSensitivity = 0.7f;

	/// <summary>
	/// Used to activate or deactivate cinema mode
	/// </summary>
	bool cinemaModeActive = false;

protected:

	/// <summary>
	/// The sprint is active
	/// </summary>
	bool sprintIsActive = false;

	/// <summary>
	/// Function called in order to initialize some values after the constructor finishes.
	/// </summary>
	void BeginPlay();

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void AddControllerPitchInput(float Val);
	void AddControllerYawInput(float Val);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/* [L3 || Shift] Called in order to start running */
	void StartRunning();

	/* [L3 || Shift] Called in order to stop running */
	void StopRunning();

	/* [R2 || Right click] Called in order to execute an attack or push trap on world */
	void MainAction();

	/* [R2 || Right click] Called in order to stop an attack or stop holding trap on world */
	void MainActionStop();

	/* [Cuadrado || E] Called in order to launch horde */
	void SecondaryAction();

	/* [Cuadrado || E] Called on release in order to cancel a horde launch */
	void SecondaryActionStop();

	/* [L2 || Left click] Called in order to aim with a weapon */
	void Reload();

	/* [L2 || Left click] Called in order to stop aiming with a weapon */
	void EndReload();

	/* [R1 || 2 ] Called in order to change the current asset that the character is holding (weapon or trap) with the right one */
	void ChangeToRightAsset();

	/* [L1 || 1 ] Called in order to change the current asset that the character is holding (weapon or trap) with the left one */
	void ChangeToLeftAsset();

	/* [ Cuadrado || -- ] Called in order to change between inventories (weapons or traps) */
	void MeleeAttack();

	/* [ Triangulo || I ] Called in order to change between inventories (weapons or traps) */
	void ChangeInventory();

	/*[ Circulo || C ] Pick items */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character")
	void GetItem();

	/* [ Touch Screen || M ] Open minimap */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character")
	void ZoomMap();
	bool isZoomMap = false;

	/* [ Start Button || Esc ] Pause the game */
	void Pause();

	/* [ X Button || Space bar ] Starts the jump */
	void StartJumping();

	/* [ X Button || Space bar ] Stops the jump */
	void StopJumping();

	/* [ F8 ] Ejects the controller from the player */
	void Eject();

	/// <summary>
	/// The main character
	/// </summary>
	ANecrognomiconCharacter* necroCharacter;

	UCharacterMovementComponent* _necroMovement;

	UNGN_GameInstance* gameInstance;
	bool shouldCheckInput = false;
	uint8 ticksNoCheckedInput = 0;

	/// <summary>
	/// The eject character used for ejecting from the player controller in the release
	/// </summary>
	AEjectCharacter* ejectCharacter;

	/// <summary>
	/// The is eject active
	/// </summary>
	bool isEjectActive = false;

	/// <summary>
	/// Activates mower movement.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "MowerWeapon")
	void ActivateMower(float mRotation, float mSpeed, float mStoppedRotation);

	/// <summary>
	/// Deactivates mower movement.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "MowerWeapon")
	void DeactivateMower();

	/// <summary>
	/// Turn on mower.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "MowerWeapon")
	void TurnOnMower();

	/// <summary>
	/// Turn off mower.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "MowerWeapon")
	void TurnOffMower();

	/// <summary>
	/// Turn on mower.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "MowerWeapon")
	void ModifyPlayerCollision();
};