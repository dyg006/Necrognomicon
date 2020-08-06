// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Managers/NGN_GnomeManager.h"
#include "Traps/BPADiscoMushroomTrap.h"

#include "NGN_GnomeController.generated.h"

// Forward declarations
class AHouse;
struct FBlackboardKeySelector;
class ANGN_Path;

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API ANGN_GnomeController : public AAIController
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vision")
		float closeRange = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
		float farRange = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
		float fovAngle = 180;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PathFollow")
		/// <summary>
		/// The farthest distance a gnome can be from the path
		/// </summary>
		float maxAstray = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFollow")
		/// <summary>
		/// The time a gnome will be ignoring the player after chasing it
		/// </summary>
		float ignoreTime = 5.0f;

	UFUNCTION(BlueprintCallable)
		void SetHouse(AHouse* house);

	UFUNCTION(BlueprintCallable)
		void SetPath(ANGN_Path* path);

	UFUNCTION(BlueprintCallable)
		void SetEndless(bool bEndless);

	UFUNCTION(BlueprintCallable)
		void RemovePath();

	void CheckPlayerPerceived();
	
	void CheckStucked(float StuckDistance);

	void AskCanAttackPlayer();

	void UnregisterFromPlayerSlot();
	
	/// <summary>
	/// Updates the PathFollowing position.
	/// </summary>
	/// <param name="followPositionKeyName">Name of the position to follow key.</param>
	/// <param name="resetKeyName">if set to <c>true</c> recalculates te total position on the path.</param>
	void UpdatePathFollowPosition(FName followPositionKeyName, FName resetPositionKeyName);


	/// <summary>
	/// Checks whether or not the actor has gone to far from the path.
	/// </summary>
	/// <param name="followPositionKeyName">Name of the position to follow key.</param>
	void CheckAstrayed(FName followPositionKeyName);

	/// <summary>
	/// Dances the specified time to dance.
	/// </summary>
	/// <param name="timeToDance">The time to dance.</param>
	void MushroomInRadius(AActor* mushroom, float timeToDance, float radius);

	void SearchDancingSpot();

	void SearchHouseSpot();

	void SearchPlayerSpot();

	/// <summary>
	/// Stops dancing.
	/// </summary>
	void StopDancing();

	UPROPERTY()
	/// <summary>
	/// The current mushroom objective
	/// </summary>
	ABPADiscoMushroomTrap* currentMushroomObjective;
	
	/// <summary>
	/// Check if the gnome is dancing or not
	/// </summary>
	bool GetIsDancing();

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Sets the AI active.
	/// </summary>
	/// <param name="active">if set to <c>true</c> [active].</param>
	void SetAIActive(bool active);

	FVector GetRandomPointNavigable(FVector origin, float radius);

	void SetCustomDepth(bool newValue);

	void JumpForward(float distance, float duration);

	FVector GetBackLocation(float Radius);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* _behaviorTree;

	UBlackboardComponent* _blackboard;

	FTimerHandle _timerHandle;

	UNavigationSystemV1* _navSys;

	float _mushroomRadius;

	UNGN_GnomeManager* _gnomeManager;

	bool _endless;

	void SetCanAttackPlayer(bool canAttack);
	
	bool GetCanAttackPlayer();

	void SetIsAstrayed(bool isAstrayed);
	
	bool GetIsAstrayed();

	void SetIsDancing(bool isAstrayed);

	void SetDancingTime(float dancingTime);

	void BeginPlay() override;

private:

	FVector m_previousLocation;
};
