// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructorHelpers.h"
//#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
//#include "GridEditor.h"
//#include "Engine/TextRenderActor.h"
#include "Tile.generated.h"

//class AGridEditor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddTileDelegate, FVector, Coords);

UENUM(BlueprintType)
enum TileSide { UP_SIDE, DOWN_SIDE, LEFT_SIDE, RIGHT_SIDE };

UENUM(BlueprintType)
enum TileType { BASE, DELETE, HOUSE, ROAD };

UENUM(BlueprintType)
enum TileRotationType { ROTATE_HORIZONTAL, ROTATE_VERTICAL };


UCLASS()
class NECROGNOMICON_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	//AGridEditor* editor;
	UMaterialInstanceDynamic* baseMaterial;
	UMaterialInstanceDynamic* roadMaterial;
	UMaterialInstanceDynamic* houseMaterial;
	UMaterialInstanceDynamic* deleteMaterial;
	UProceduralMeshComponent* mesh;
	UTextRenderComponent* text;
	UPROPERTY(EditAnywhere) int sideSize;
	UPROPERTY(EditAnywhere) FVector coords;
	TArray<FVector> _vertices_base;
	TArray<FVector> _vertices;

public:

	UPROPERTY(EditAnywhere) TEnumAsByte<TileType> type;
	UPROPERTY(EditAnywhere) int heightLevel;

	//Variables para controlar la inclinación del tile
	UPROPERTY(EditAnywhere, Category = "Tile|Shape", META = (UIMin = -45, UIMax = 45)) float rotateHorizontal;
	UPROPERTY(EditAnywhere, Category = "Tile|Shape", META = (UIMin = -45, UIMax = 45)) float rotateVertical;

	//Variables para magnetizar un lado del tile
	UPROPERTY(EditAnywhere, Category = "Tile|Shape") bool fixedUp;
	UPROPERTY(EditAnywhere, Category = "Tile|Shape") bool fixedDown;
	UPROPERTY(EditAnywhere, Category = "Tile|Shape") bool fixedLeft;
	UPROPERTY(EditAnywhere, Category = "Tile|Shape") bool fixedRight;


	void Init(float size, FVector coords,
		UMaterialInterface* baseMaterial, UMaterialInterface* roadMaterial, UMaterialInterface* houseMaterial, UMaterialInterface* deleteMaterial
		//,AGridEditor* editor
	);

	FVector computeNormal(const FVector p1, const FVector p2, const FVector p3) const;

	void setMesh(TArray<FVector> vertices, bool update);
	void setMaterial();
	void setText(const int x, const int y, const int z);
	void rotateMesh(TileRotationType rotationType);

	// Funciones de gestion de la malla
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tile", DisplayName = "Up") void addTileUp() const;
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tile", DisplayName = "Down") void addTileDown() const;
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tile", DisplayName = "Left") void addTileLeft() const;
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tile", DisplayName = "Right") void addTileRight() const;
	void addTile(TileSide side) const;


	//Definimos un delegado para informar al GridEditor cuando tiene que añadir un tile
	FAddTileDelegate AddTileOnDelegate;

	UFUNCTION(BlueprintCallable, Category = "Tile") bool CheckCellFree(const FVector objCoords);


#if WITH_EDITOR  
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
