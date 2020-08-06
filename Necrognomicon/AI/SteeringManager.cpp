// Fill out your copyright notice in the Description page of Project Settings.


#include "SteeringManager.h"


// Sets default values for this component's properties
USteeringManager::USteeringManager() : 
	steering(FVector(0.f, 0.f, 0.f)), position(FVector(0.f, 0.f, 0.f)), velocity(FVector(0.f, 0.f, 0.f)),
	maxSpeed(FVector(0.f,0.f,0.f)), maxForce(FVector(0.f,0.f,0.f)), mass(1.f),
	neighborRadius(100.f), slowingRadius(10.f),
	wanderAngle(10.f), wanderCircleDistance(10.f), wanderAngleChange(10.f), 
	avoidanceMaxSeeAhead(10.f) ,avoidanceMaxForce(10.f),
	wAlignment(1.f), wCohesion(1.f), wSeparation(1.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USteeringManager::BeginPlay()
{
	Super::BeginPlay();
	ownerActor = GetOwner();
	reset();
}


// Called every frame
void USteeringManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector USteeringManager::getVelocity() const {
	return velocity;
}

void setAngle(FVector* vector, float value) {
	float len = vector->Size();
	vector->X = cos(value) * len;
	vector->Y = sin(value) * len;
}

FVector truncate(const FVector v1, const FVector v2) {
	return {
		FMath::Min(v1.X, v2.X),
		FMath::Min(v1.Y, v2.Y),
		FMath::Min(v1.Z, v2.Z)
	};
}

void USteeringManager::findNeighbors() {
	// Retrieve all the agents of the same class as the owner
	TArray<AActor*> allAgents;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ownerActor->GetClass(), allAgents);

	// Reset the neighbors array and add those within range
	neighborAgents.Empty();
	for (int i = 0; i <= allAgents.Num(); ++i) {
		if (allAgents[i] != ownerActor) {
			if ((ownerActor->GetTransform().GetLocation() - allAgents[i]->GetTransform().GetLocation()).Size() < neighborRadius) {
				neighborAgents.Add(allAgents[i]);
			}
		}
	}
}


void USteeringManager::reset() {
	// Null vector, meaning zero force
	steering = { .0f, .0f, .0f };

	// Get agent current position
	position = ownerActor->GetTransform().GetLocation();
}

FVector USteeringManager::update() {

	//TODO: comprobar si tenemos que ajustar la velocidad con el tiempo
	//TODO: comprobar si tenemos que utilizar la orientacion del personaje

	// The steering force is truncated to ensure it will not exceed
	// the amount of allowed forces the character can handle.
	steering = truncate(steering, maxForce);

	// The steering force is divided by the character mass, which produces different movement
	// speeds for different weighted characters.
	steering /= mass;

	//
	velocity = truncate(velocity + steering, maxSpeed);
	position += velocity;

	return position;
}

void USteeringManager::seek(const FVector target) {
	steering += doSeek(target);
}

void USteeringManager::flee(const FVector target) {
	steering += doFlee(target);
}

void USteeringManager::arrival(const FVector target) {
	steering += doArrival(target);
}

void USteeringManager::wander() {
	steering += doWander();
}

void USteeringManager::pursuit(const FVector target, const FVector targetVelocity) {
	steering += doPursuit(target, targetVelocity);
}

void USteeringManager::evade(const FVector target, const FVector targetVelocity) {
	steering += doEvade(target, targetVelocity);
}



FVector USteeringManager::doSeek(const FVector target) const {
	return ((target - position).Normalize() * maxSpeed) - velocity;
}

FVector USteeringManager::doFlee(const FVector target) const {
	return ((position - target).Normalize() * maxSpeed) - velocity;
}

FVector USteeringManager::doArrival(const FVector target) const {
	//Calculate the desired velocity
	FVector desired_velocity = target - position;
	float distance = desired_velocity.Size();

	//Check the distance to detect whether the character is inside the slowing area
	desired_velocity = desired_velocity.Normalize() * maxSpeed * ((distance < slowingRadius) ? (distance / slowingRadius) : 1);

	return desired_velocity - velocity;
}

FVector USteeringManager::doWander() {
	
	// Calculate the circle center
	FVector circleCenter = velocity;
	circleCenter.Normalize();
	circleCenter * wanderCircleDistance;

	// Calculate the displacement force
	FVector displacement = {.0f, -1.f, .0f};
	displacement * wanderCircleDistance;

	// Randomly change the vector direction by making it change its current angle
	setAngle(&displacement, wanderAngle);

	// Change wanderAngle just a bit, so it won't have the same value in the next game frame.
	wanderAngle += rand() * wanderAngleChange - wanderAngleChange * .5f;

	//Finally calculate and return the wander force;
	return circleCenter + displacement;
}

FVector USteeringManager::doPursuit(const FVector target, const FVector targetVelocity) const {
	int ticks = (target - position).Size() / maxSpeed.Size();
	return doSeek(target + targetVelocity * ticks);
}

FVector USteeringManager::doEvade(const FVector target, const FVector targetVelocity) const {
	int ticks = (target - position).Size() / maxSpeed.Size();
	return doFlee(target + targetVelocity * ticks);
}



FVector USteeringManager::doAlignment() const {

	FVector v = FVector();
	if (neighborAgents.Num() == 0)
		return v;

	FVector neighborVelocity;
	for (int i = 0; i < neighborAgents.Num(); ++i) {
		// Retrieve the neighbor's steering manager component
		TArray<USteeringManager*> steManagerComps;
		neighborAgents[i]->GetComponents<USteeringManager>(steManagerComps);
		if (steManagerComps.Num() > 0) {
			neighborVelocity = steManagerComps[0]->getVelocity();
			v.X += neighborVelocity.X;
			v.Y += neighborVelocity.Y;
			v.Z += neighborVelocity.Z;
		}
	}

	v /= neighborAgents.Num();
	//v = v - position;
	v.Normalize(1);
	return v;
}

FVector USteeringManager::doCohesion() const {

	FVector v = FVector();
	if (neighborAgents.Num() == 0)
		return v;

	FVector neighborPosition;
	for (int i = 0; i < neighborAgents.Num(); ++i) {
		neighborPosition = neighborAgents[i]->GetTransform().GetLocation();
		v.X += neighborPosition.X;
		v.Y += neighborPosition.Y;
		v.Z += neighborPosition.Z;
	}

	v /= neighborAgents.Num();
	v = v - position;
	v.Normalize(1);
	return v;
}

FVector USteeringManager::doSeparation() const {
	
	FVector v = FVector();
	if (neighborAgents.Num() == 0)
		return v;

	FVector neighborPosition;
	for (int i = 0; i < neighborAgents.Num(); ++i) {
		neighborPosition = neighborAgents[i]->GetTransform().GetLocation();
		v.X += neighborPosition.X - position.X;
		v.Y += neighborPosition.Y - position.Y;
		v.Z += neighborPosition.Z - position.Z;
	}

	v /= neighborAgents.Num();
	v = v - position;
	v.Normalize(1);
	return v;
}

void USteeringManager::flocking() {
	FVector alignment = doAlignment();
	FVector cohesion = doCohesion();
	FVector separation = doSeparation();
	velocity.X += alignment.X * wAlignment + cohesion.X * wCohesion + separation.X * wSeparation;
	velocity.Y += alignment.Y * wAlignment + cohesion.Y * wCohesion + separation.Y * wSeparation;
	velocity.Z += alignment.Z * wAlignment + cohesion.Z * wCohesion + separation.Z * wSeparation;
	velocity = velocity.Normalize() * maxSpeed;
}







void USteeringManager::collisionAvoidance() { 
	/*
	float dynamic_length = velocity.Size() / maxSpeed.Size();
	FVector ahead = position + velocity.Normalize() * dynamic_length;
	//FVector ahead = position + velocity.Normalize() * avoidanceMaxSeeAhead;
	FVector ahead2 = position + velocity.Normalize() * avoidanceMaxSeeAhead * 0.5;

	AActor* mostThreatening = findMostThreateningObstacle();
	FVector avoidance = {0,0,0};

	if (mostThreatening) {
		avoidance.X = ahead.X - mostThreatening.center.x;
		avoidance.Y = ahead.Y - mostThreatening.center.y;
		avoidance = avoidance.Normalize() * avoidanceMaxForce;
	}

	return avoidance;
	*/
	//avoidance_force = ahead - obstacle_center;
	//avoidance_force = avoidance_force.Normalize() * avoidanceMaxForce;

}

bool lineIntersectsCircle(const FVector ahead, const FVector ahead2, AActor * obstacle){
	/*
	// the property "center" of the obstacle is a Vector3D.
	return (obstacle.center-ahead).Size() <= obstacle.radius || (obstacle.center-ahead2).Size() <= obstacle.radius;
	*/
	return false;
}

AActor* USteeringManager::findMostThreateningObstacle(){
	
	AActor* mostThreatening = nullptr;
	/*
	for (int i = 0; i < Game.instance.obstacles.length; i++) {
		AActor* obstacle = Game.instance.obstacles[i];
		bool collision = lineIntersecsCircle(ahead, ahead2, obstacle);

		// "position" is the character's current position
		if (collision && (mostThreatening == nullptr || (position-obstacle).Size() < (position-mostThreatening).Size())) {
			mostThreatening = obstacle;
		}
	}
	*/
	return mostThreatening;
}


void USteeringManager::pathFollowing() {  }
void USteeringManager::leaderFollowing() {  }
void USteeringManager::queue() { }
