// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "math.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SteeringManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROGNOMICON_API USteeringManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USteeringManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	
	// The steering force to apply
	FVector steering;
	
	// Owner of the steering manager
	AActor* ownerActor;
	// Owner's current position
	FVector position;
	// Owner's current velocity vector
	FVector velocity;

	// Array of neighbors
	TArray<AActor*> neighborAgents;

	//-------------------------------
	// Internal API
	//-------------------------------
	FVector doSeek(const FVector target) const;
	FVector doFlee(const FVector target) const;
	FVector doArrival(const FVector target) const;
	FVector doWander();
	FVector doEvade(const FVector target, const FVector targetVelocity) const;
	FVector doPursuit(const FVector target, const FVector targetVelocity) const;
	FVector doSeparation() const;
	FVector doCohesion() const;
	FVector doAlignment() const;
	FVector doFlocking() const;

	void findNeighbors();

public:

	// Owner's maximum Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		FVector maxSpeed;
	// Maximum steering force allowed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		FVector maxForce;
	// Owner's mass. Affects the force inversely
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float mass;
	// Owner's radius to search for neighbors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float neighborRadius;

	//-------------------------------
	// Arrival
	//-------------------------------
	// Radius to slowDown around the waypoints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Arrival")
		float slowingRadius;

	//-------------------------------
	// Wandering
	//-------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Wander", meta = (UIMin = "0", UIMax = "260"))
		float wanderAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Wander")
		float wanderCircleDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Wander", meta = (UIMin = "0", UIMax = "360"))
		float wanderAngleChange;

	//-------------------------------
	// Avoidance
	//-------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Avoidance")
		float avoidanceMaxSeeAhead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Avoidance")
		float avoidanceMaxForce;

	AActor* findMostThreateningObstacle();


	//-------------------------------
	// Flocking
	//-------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Flocking", meta = (UIMin = "0", UIMax = "1"))
		float wAlignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Flocking", meta = (UIMin = "0", UIMax = "1"))
		float wCohesion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors|Flocking", meta = (UIMin = "0", UIMax = "1"))
		float wSeparation;



	// The update method. 
	// Should be called after all behaviors have been invoked
	FVector update();

	// Reset the internal steering force.
	void reset();

	FVector getVelocity() const;

	//-------------------------------
	// External API
	//-------------------------------
	void seek(const FVector target);
	void flee(const FVector target);
	void arrival(const FVector target);
	void wander();
	void pursuit(const FVector target, const FVector targetVelocity);
	void evade(const FVector target, const FVector targetVelocity);

	void alignment();
	void cohesion();
	void separation();
	void flocking();

	void collisionAvoidance();
	void pathFollowing();
	void leaderFollowing();
	void queue();
};
