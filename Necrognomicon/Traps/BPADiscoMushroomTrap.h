// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/BPATrap.h"
#include "Enemy/NecrognomiconEnemy.h"
#include "BPADiscoMushroomTrap.generated.h"

class ANGN_DiscoMusicActorManager;

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API ABPADiscoMushroomTrap : public ABPATrap
{
	GENERATED_BODY()

public:
    ABPADiscoMushroomTrap();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /// <summary>
    /// Notifies the trap when an actor begin overlapping with it.
    /// </summary>
    /// <param name="OtherActor">The actor that causes the overlap.</param>
    void NotifyActorBeginOverlap(AActor* OtherActor) override;

    /// <summary>
   /// Notifies the trap when an actor begin overlapping with it.
   /// </summary>
   /// <param name="OtherActor">The actor that causes the overlap.</param>
    void NotifyActorEndOverlap(AActor* OtherActor) override;

    /// <summary>
    /// Charms an enemy in order to make him dance around the trap.
    /// </summary>
    void CharmEnemy(AActor* enemy);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    /// <summary>
    /// The maximum number of enemies that can be charmed by the trap
    /// </summary>
    int maxNumberOfEnemies = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    /// <summary>
    /// The time to dance
    /// </summary>
    float timeToDance = 5.0f;

    /// <summary>
    /// Enemies the finished dancing.
    /// </summary>
    void EnemyFinishedDancing();

    /// <summary>
    /// Enemies the finished dancing.
    /// </summary>
    void EnemyStartedDancing();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USkeletalMesh* EnabledTrapSkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USkeletalMesh* DisabledTrapSkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UAnimationAsset* DanceAnimation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USkeletalMeshComponent* FloorSkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UAnimationAsset* ConstructionAnimationMushroom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UAnimationAsset* ConstructionAnimationFloor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* AreaCollision;
 
    /// <summary>
    /// List of charmed enemies
    /// </summary>
    TArray<AActor*> CharmedEnemies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UFMODAudioComponent* danceMusicAudioComponent;

private:

    /// <summary>
   /// The dance sound component used to synchronize discoMushroom songs
   /// </summary>
    TArray<UActorComponent*> danceSoundComponents;

    /// <summary>
    /// The disco actor manager
    /// </summary>
    ANGN_DiscoMusicActorManager* discoActorManager;

    /// <summary>
    /// Check if the trap is active
    /// </summary>
    bool IsActive = true;

    /// <summary>
    /// List of overlaping enemies
    /// </summary>
    TArray<AActor*> OverlapingEnemies;

    /// <summary>
    /// The current enemies dancing
    /// </summary>
    int currentEnemiesDancing = 0;

    /// <summary>
    /// Checks if can there are enemies on range.
    /// </summary>
    void CheckIfCanCharmEnemy();

    /// <summary>
    /// Deactivates the trap.
    /// </summary>
    void DeactivateTrap();

    /// <summary>
    /// Activates the trap.
    /// </summary>
    void ActivateTrap();

    /// <summary>
    /// Activates the dance animation.
    /// </summary>
    void ActivateDanceAnimation();
    void changeMeshVisibility();
    void changeFloorVisibility();

    /// <summary>
    /// The timer delay before activation
    /// </summary>
    FTimerHandle TimerHandle_DiscoMushroomDance;

    /// <summary>
    /// The delay before start dancing
    /// </summary>
    float DelayBeforeStartDancing = 1.6;

    /// <summary>
    /// Check if the trap is active
    /// </summary>
    bool active = false;

    /// <summary>
    /// We are in placing state
    /// </summary>
    bool placingState = false;
};
