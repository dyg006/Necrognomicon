// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/NecrognomiconEnemy.h"
#include "NGN_GnomeManager.generated.h"

class APlayerAttackingSlot;

USTRUCT(BlueprintType)
struct NECROGNOMICON_API FAttackingGnomes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<ANecrognomiconEnemy>, int32> maxGnomesAttackingPlayer;
};


/**
 * 
 */
UCLASS(Blueprintable)
class NECROGNOMICON_API UNGN_GnomeManager : public UObject
{
	GENERATED_BODY()

public:
	
	bool TryAttackPlayer(TSubclassOf<ANecrognomiconEnemy> gnomeType);

	void UnregisterGnome(TSubclassOf<ANecrognomiconEnemy> gnomeType, APlayerAttackingSlot* playerSlot);

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(int32 level) { _currentLevel = level - 1; }

	UFUNCTION(BlueprintCallable)
	void ResetCurrentGnomes();

	int32 GetTotalMaxGnomes();

	int32 GetTotalCurrentGnomes();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray <FAttackingGnomes> _maxGnomesAttackingPlayerPerLevel;

	UPROPERTY(EditAnywhere, DisplayName= "NO TOCAR", Category= "NO TOCAR")
	TMap<TSubclassOf<ANecrognomiconEnemy>, int32> _currentAttackingGnomes;

	UPROPERTY(VisibleAnywhere)
	int32 _currentLevel = 0;

	int32 GetCurrentGnomes(TSubclassOf<ANecrognomiconEnemy> gnomeType);

	void RegisterGnome(TSubclassOf<ANecrognomiconEnemy> gnomeType);
	
	int32 GetMaxGnomes(TSubclassOf<ANecrognomiconEnemy> gnomeType);
};
