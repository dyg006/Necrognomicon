// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h" 
#include "GameFramework/Actor.h"
#include "HealthComponent.generated.h"

class ANecrognomiconCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROGNOMICON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Apply Damage to Health
	float Damage(float ActualDamage);

	void Heal(float healingPoints);

	float GetHealthPercentage() const { return health / defaultHealth; }

	void Init(float maxHealth);

	bool IsFullHealth() const { return defaultHealth == health; }

	/// <summary>
	/// Variable used to established the initial health of the character
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float defaultHealth;

	// Variable used to established the current health of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float health;

	// Variable used to check if the character is alive or not
	bool alive;
};
