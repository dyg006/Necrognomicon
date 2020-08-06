// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/BPAObject.h"
#include "Tools/NGN_Grid.h"
#include "Materials/MaterialInterface.h"
#include "Inventory/InventoriableInterface.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "BPATrap.generated.h"

UCLASS()
class NECROGNOMICON_API ABPATrap : public ABPAObject, public IInventoriableInterface
{
	GENERATED_BODY()

private:

	void InvalidGridPosition();

	class ANecrognomiconGameMode* gameMode;

	class ANecrognomiconCharacter* player;

public:	
	// Sets default values for this actor's properties
	ABPATrap();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traps")
	float HIGHLIGHT_DISTANCE;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traps")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceGridType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	ANGN_Grid* Grid;
	//TSubclassOf<ANGN_Grid> Grid;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traps")
	//bool InGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	int32 Column;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traps")
	TArray<UMaterialInterface*> BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	UMaterialInterface* GreenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	class UAnimationAsset* ActivateAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	class UAnimationAsset* ConstructionAnimation;

	// Trap properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Sounds")
		UFMODEvent* ActivationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
		UFMODEvent* ConstructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
		TArray<TSubclassOf<AActor>> ActivationActors; // lo que activa la trampa
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Properties")
		TArray<TSubclassOf<AActor>> DamageReceivers; // los actores que reciben el daño
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Traps")
		FTransform ResetTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
		/// <summary>
		/// The base ren animation blueprint
		/// </summary>
		UClass* BaseRenAnimationBlueprint;

	// Wait for grids to initialize
	FTimerHandle AutoPlaceTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Execute action
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	//UFUNCTION(BlueprintImplementableEvent, meta = Category = "Activation")
	virtual void action();

	UFUNCTION(BlueprintCallable, Category = "Highligth")
	void ChangeFocusStatus(bool IsPlayerOverlap);
	void BeginFocus();
	void EndFocus();

	UFUNCTION(BlueprintCallable, Category = "Traps")
	void PlaceObjectCpp();

	UPROPERTY(BlueprintReadOnly)
		bool OverGrid = false;
	
	UPROPERTY(BlueprintReadWrite)
		bool IsActive = false;

	UFUNCTION(BlueprintCallable, Category = "Traps")
	virtual void UpdatePlacingPositionCpp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void Select();
	virtual void Select_Implementation() override;

	// Execute action
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void Execute();
	virtual void Execute_Implementation() override;

	// Pick up the object
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
		void Interaction();
	virtual void Interaction_Implementation() override;

	virtual void Reset() override;

private:
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void AutoPlaceObjectCpp();

};
