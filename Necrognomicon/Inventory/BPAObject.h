// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPAObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsPlayerOverlap, bool, isOverlapping);

UENUM(BlueprintType)
enum EBPLTipo
{
	Weapon     UMETA(DisplayName = "WEAPON"),
	Trap      UMETA(DisplayName = "TRAP"),
};

USTRUCT(BlueprintType)
struct FBPEObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Nombre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Descripcion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABPAObject> Clase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Imagen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ImagenSecundaria;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EsApilable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaximoApilable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBPLTipo> Tipo;
};

UCLASS()
class NECROGNOMICON_API ABPAObject : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABPAObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hightlight")
		float HightlightDistance;

	UPROPERTY()
	bool Placing = false;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	bool CanBePlaced = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FBPEObject EstructuraObjeto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		int VideoIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		FText Message;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool IsVideoEnabled = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
		bool IsInInventory = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UsarObjeto();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void StartFocus();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void EndFocus();

	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
		FIsPlayerOverlap PlayerOverlapEvent;
	
};
