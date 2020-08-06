// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/BPAObject.h"
#include "VideoTutorialManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FShowTutorial, int, index, FText, message, bool, hasVideo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowObjectTutorial, ABPAObject*, object);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFirstObject);

UCLASS(Blueprintable)
class NECROGNOMICON_API UVideoTutorialManager : public UObject
{
	GENERATED_BODY()
	
public:
	UVideoTutorialManager();
	~UVideoTutorialManager();

	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
		FShowTutorial ShowTutorialEvent;
	
	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
		FShowObjectTutorial ShowObjectTutorialEvent;
	
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
		FFirstObject FFirstObjectEvent;

	TArray<UClass*> IgnoreClassList;
	TArray<UClass*> RevelaedEnemiesClassList;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ABPAObject*> InventoryObjects;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ANecrognomiconEnemy*> RevealedEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTutorialEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFirstObject = true; //Solo se llama la primera vez

public:

	void ResetManager();

	UFUNCTION(BlueprintCallable)
	void GetObjectsClass(class ABPAObject* Object);
	
	UFUNCTION(BlueprintCallable)
	void UpdateGlossaryMailBox(class ABPAObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Enemies")
	void UpdateRevealedEnemisList(class ANecrognomiconEnemy* NewEnemy);
};
