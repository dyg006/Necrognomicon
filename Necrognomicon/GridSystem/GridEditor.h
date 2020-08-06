// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tile.h"
#include "GridEditor.generated.h"

//class ATile;
//enum class TileSide;


UCLASS()
class NECROGNOMICON_API AGridEditor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridEditor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	TArray<AActor*> tiles;
	int _hSize;
	int _vSize;
	int _nTiles;

public:
	UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<AActor> _actorToSpawn;
	UPROPERTY(EditAnywhere, Category = "Grid|Shape") int _width;
	UPROPERTY(EditAnywhere, Category = "Grid|Shape") int _height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Shape") int _poligonSize;

	UPROPERTY(EditAnywhere, Category = "Grid|Materials", DisplayName = "Base Material") UMaterialInterface* baseMaterial;
	UPROPERTY(EditAnywhere, Category = "Grid|Materials", DisplayName = "Road Material") UMaterialInterface* roadMaterial;
	UPROPERTY(EditAnywhere, Category = "Grid|Materials", DisplayName = "House Material") UMaterialInterface* houseMaterial;
	UPROPERTY(EditAnywhere, Category = "Grid|Materials", DisplayName = "Delete Material") UMaterialInterface* deleteMaterial;


	bool addTile(const FVector coords);

	//Delegate Function that binds the tiles messages
	UFUNCTION() void BindToDelegate(FVector Coords);

private:


#if WITH_EDITOR  
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Grid") void GenerateGrid();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Grid") void DestroyGrid();
	//UFUNCTION(BlueprintCallable, CallInEditor, Category = "Grid") void LoadFiles();
	UFUNCTION(BlueprintCallable, Category = "Grid") int GetIndexOfCell(const FVector coords);
	UFUNCTION(BlueprintCallable, Category = "Grid") FTransform GetCellTransform(const int indexOfCell);

	void GenerateTiles();
	void DestroyTiles();

};
