// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_GnomeManager.h"
#include "Components/PlayerAttackingSlot.h"

bool UNGN_GnomeManager::TryAttackPlayer(TSubclassOf<ANecrognomiconEnemy> gnomeType)
{
	if (GetCurrentGnomes(gnomeType) < GetMaxGnomes(gnomeType))
	{
		RegisterGnome(gnomeType);
		return true;
	}
	return false;
}


int32 UNGN_GnomeManager::GetCurrentGnomes(TSubclassOf<ANecrognomiconEnemy> gnomeType)
{
	return _currentAttackingGnomes[gnomeType];
}

int32 UNGN_GnomeManager::GetMaxGnomes(TSubclassOf<ANecrognomiconEnemy> gnomeType)
{
	return _maxGnomesAttackingPlayerPerLevel[_currentLevel].maxGnomesAttackingPlayer[gnomeType];
}

int32 UNGN_GnomeManager::GetTotalMaxGnomes()
{
	int totalGnomes = 0;
	for (auto& gnomeType : _maxGnomesAttackingPlayerPerLevel[_currentLevel].maxGnomesAttackingPlayer) {
		totalGnomes += gnomeType.Value;
	}
	return totalGnomes;
}

void UNGN_GnomeManager::RegisterGnome(TSubclassOf<ANecrognomiconEnemy> gnomeType)
{
	_currentAttackingGnomes[gnomeType]++;
}

void UNGN_GnomeManager::UnregisterGnome(TSubclassOf<ANecrognomiconEnemy> gnomeType, APlayerAttackingSlot* playerSlot)
{
	if (GetCurrentGnomes(gnomeType) > 0)
	{
		//check(playerSlot);
		if (playerSlot != nullptr) {
			playerSlot->isFree = true;
		}
		_currentAttackingGnomes[gnomeType]--;
	}
}

void UNGN_GnomeManager::ResetCurrentGnomes()
{
	for (auto& gnomeType : _currentAttackingGnomes)
	{
		gnomeType.Value = 0;
	}
}

int32 UNGN_GnomeManager::GetTotalCurrentGnomes() {
	int totalGnomes = 0;
	for (auto& gnomeType : _maxGnomesAttackingPlayerPerLevel[_currentLevel].maxGnomesAttackingPlayer) {
		totalGnomes += GetCurrentGnomes(gnomeType.Key);
	}
	return totalGnomes;
}