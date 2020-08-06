// Fill out your copyright notice in the Description page of Project Settings.


#include "VideoTutorialManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/BPAObject.h"
#include "UObject/UObjectGlobals.h"
#include "Enemy/NecrognomiconEnemy.h"

UVideoTutorialManager::UVideoTutorialManager()
{

}

UVideoTutorialManager::~UVideoTutorialManager()
{
}

void UVideoTutorialManager::GetObjectsClass(ABPAObject* Object)
{

	//ABPAObject* const CurrentObj = Cast<ABPAObject>(Object);

	if (IsFirstObject)
	{
		FFirstObjectEvent.Broadcast();
		IsFirstObject = false;
	}


	if (!IgnoreClassList.Contains(Object->GetClass()))
	{
		IgnoreClassList.Add(Object->GetClass());

		ABPAObject* copy = DuplicateObject(Object, Object->GetOuter());
		InventoryObjects.Add(copy);

		if (IsTutorialEnabled)
			ShowTutorialEvent.Broadcast(Object->VideoIndex, Object->Message, Object->IsVideoEnabled);
	}	
}

void UVideoTutorialManager::UpdateGlossaryMailBox(ABPAObject* Object)
{
	if (!IgnoreClassList.Contains(Object->GetClass()))
	{
		IgnoreClassList.Add(Object->GetClass());

		ABPAObject* copy = DuplicateObject(Object, Object->GetOuter());
		InventoryObjects.Add(copy);
	}
}

void UVideoTutorialManager::ResetManager()
{
	IsFirstObject = true;
	IgnoreClassList.Empty();
	RevelaedEnemiesClassList.Empty();
}

void UVideoTutorialManager::UpdateRevealedEnemisList(ANecrognomiconEnemy* NewEnemy)
{
	if (!RevelaedEnemiesClassList.Contains(NewEnemy->GetClass())) {
		RevelaedEnemiesClassList.Add(NewEnemy->GetClass());

		ANecrognomiconEnemy* copyEnemy = DuplicateObject(NewEnemy, NewEnemy->GetOuter());
		RevealedEnemies.Add(copyEnemy);
	}
}
