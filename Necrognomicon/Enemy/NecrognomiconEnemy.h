// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"
#include "DestructibleActor.h"
#include "GameFramework/DamageType.h"
#include "NecrognomiconEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FResidualDamageDelegate, float, Damage, float, MotionModifier, float, ActiveTime, AActor*, DamageCauser);

USTRUCT(BlueprintType)
struct FBPEnemyData
{
	GENERATED_USTRUCT_BODY()

public:
	FBPEnemyData() {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

};

UCLASS()
class NECROGNOMICON_API ANecrognomiconEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANecrognomiconEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle for efficient management of OnTimerTick timer
	FTimerHandle DestroyTimerHandle;

	// Destroy original actor
	virtual void DestroyTimer(FVector Direction);

	/// <summary>
	/// The common gnome character animation
	/// </summary>
	UAnimInstance* enemyAnimationInstance;

	/// <summary>
	/// Attack anim montage of the common gnome character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* attackAnimMontage;

	/// <summary>
	/// Attack anim montage of the common gnome character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimSequence* danceAnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo")
	FBPEnemyData EnemyInfo;

	/// <summary>
	/// Enemy skeletal mesh component
	/// </summary>
	USkeletalMeshComponent* enemyMeshComponent = nullptr;

	/// <summary>
	/// Dynamic material to change its properties in runtime
	/// </summary>
	UMaterialInstanceDynamic* dynamicMaterial = nullptr;

	/// <summary>
	/// Variable used to check if the gnome has attacked
	/// </summary>
	bool hasAttacked = false;

	//Used to self destruct if it was launched but has not since detected a collision
	float TimeLaunched;
	float TimeToLiveSinceLaunch = 3.f;

	UPROPERTY(EditAnywhere)
	float _attackRange = 125.f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;


	/*Sets whether the enemy will be launched by melee (true) or receive damage directly (false)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	bool IsLaunchedByMelee = true;

	/*Used to detect collision after launch*/
	bool HasBeenLaunched = false;

	UFUNCTION(BlueprintCallable)
	void LaunchSelf(FVector ImpulseVector, AActor* DamageCauser);

	void JumpForward(float Distance, float Duration);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	TSubclassOf<class ADestructibleActor> DestructibleActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float Impulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	FVector SpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	FVector ImpactPointOffset;

	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float DestroyDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float PlayerDamage;

	/** Controller that gets credit for any damage caused by this volume */
	UPROPERTY()
		class AController* DamageInstigator;


	// Damage sigle actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CausePainTo(class AActor* enemy, bool useDistance=false);

	float GetDistToEnemy(AActor* enemy) const;

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	/// <summary>
	/// Attacks this instance.
	/// </summary>
	void Attack( AActor* enemyToAttack);

	/// <summary>
	/// Plays the dance animation.
	/// </summary>
	void StartDancing();

	/// <summary>
	/// Plays the dance animation.
	/// </summary>
	void StopDancing();

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	AActor* attackObjective;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	UFMODEvent* laughSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	UFMODEvent* crashSound;

	/*
	Control daño residual
	*/
	UPROPERTY(BlueprintAssignable, Category = "Test")
	FResidualDamageDelegate ResidualDamageEvent;
	FTimerHandle TimerHandle_ResidualDamageTimer;
	FTimerHandle TimerHandle_ReceiveDamageTimer;
	float ResidualDamageAmount;
	float SpeedModifier;
	bool isSpeedModified;
	bool hasResidualDamage;
	class UCharacterMovementComponent* _characterMovement;
	AActor* ResidualDamageCauser;

	UFUNCTION(BlueprintCallable, Category = "Damage")
		void StartResidualDamage(float Damage, float MotionModifier, float ActiveTime, AActor* DamageCauser);
	void StopResidualDamage();
	void MakeResidualDamage();
	
	virtual void Reset() override;

	float activeContrast_fire;

	float activeContrast_ice;

	UPROPERTY(EditAnywhere)
	float activeEmissive_fire;

	UPROPERTY(EditAnywhere)
	float activeEmissive_ice;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Animation")
	/// <summary>
	/// Starts the dance animation.
	/// </summary>
	void StartDanceAnimation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Animation")
	/// <summary>
	/// Stops the dance animation.
	/// </summary>
	void StopDanceAnimation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float ErodeSpeed = 0.4f;

private:
	float defaultContrast_fire;

	float defaultContrast_ice;

	float defaultEmissive_fire;

	float defaultEmissive_ice;

	float defaultErode;
	float currentErode = 2.0f;
};
