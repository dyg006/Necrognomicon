// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() : defaultHealth(100), alive(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	health = defaultHealth;
}

void UHealthComponent::Init(float maxHealth)
{
	defaultHealth = maxHealth;
	health = defaultHealth;

	alive = true;
}


float UHealthComponent::Damage(float ActualDamage)
{
	health -= ActualDamage;

	float realDamage = ActualDamage;

	if (health <= 0) {
		alive = false;
		realDamage = ActualDamage + health;
		health = 0;
	}

	return realDamage;
}

void UHealthComponent::Heal(float healingPoints)
{
	health = FMath::Min(health + healingPoints, defaultHealth);
}

