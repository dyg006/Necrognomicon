// Fill out your copyright notice in the Description page of Project Settings.


#include "GridEditor.h"

//#include "HAL/PlatformFilemanager.h"
//#include "Misc/LocalTimestampDirectoryVisitor.h"

//#include <iostream>
//#include <sstream>
//#include <fstream>
//#include <string>

// Sets default values
AGridEditor::AGridEditor() : _width(2), _height(2), _poligonSize(100)
{
	baseMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("BaseMaterial"));
	roadMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("RoadMaterial"));
	houseMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("HouseMaterial"));
	deleteMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("DeleteMaterial"));
}

// Called when the game starts or when spawned
void AGridEditor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridEditor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR  
void AGridEditor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

	//Get all of our components  
	TArray<UActorComponent*> MyComponents;
	GetComponents(MyComponents);

	//Get the name of the property that was changed  
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// We test using GET_MEMBER_NAME_CHECKED so that if someone changes the property name  
	// in the future this will fail to compile and we can update it.  
	//if ((PropertyName == GET_MEMBER_NAME_CHECKED(AGridEditor, MyMesh)))ç
	if (
		(PropertyName == GET_MEMBER_NAME_CHECKED(AGridEditor, _width))
		|| (PropertyName == GET_MEMBER_NAME_CHECKED(AGridEditor, _height))
		|| (PropertyName == GET_MEMBER_NAME_CHECKED(AGridEditor, _poligonSize))
		)
	{
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AGridEditor::GenerateGrid() {
	//FMath::RandInit(_randomSeed);
	_hSize = _width;
	_vSize = _height;
	_nTiles = _vSize * _hSize;
	GenerateTiles();
}

void AGridEditor::DestroyGrid() {
	DestroyTiles();
}

void AGridEditor::GenerateTiles() {
	//Reinicializar vector de actores
	DestroyTiles();

	//Spawnear actores y añadir a la lista de actores
	for (int i = 0; i < _nTiles; ++i) {
		if (!addTile(FVector((i % _width), (i / _width), 0.f) * _poligonSize)) break;

	}
}

bool AGridEditor::addTile(const FVector coords) {
	if (_actorToSpawn) {
		UWorld* world = GetWorld();
		if (world) {
			FVector location = GetActorLocation() + coords;
			FRotator rotation = GetActorRotation();

			//AActor* tile = world->SpawnActor(_actorToSpawn, &location, &rotation);
			ATile* tile = (ATile*)world->SpawnActor(_actorToSpawn, &location, &rotation);
			tiles.Add(tile);
			//((ATile*)tile)->AddTileOnDelegate.AddDynamic(this, &AGridEditor::BindToDelegate);
			//((ATile*)tile)->Init(_poligonSize, coords, baseMaterial, roadMaterial, houseMaterial, deleteMaterial);
			tile->AddTileOnDelegate.AddDynamic(this, &AGridEditor::BindToDelegate);
			tile->Init(_poligonSize, coords, baseMaterial, roadMaterial, houseMaterial, deleteMaterial);

			return true;
		}
	}
	return false;
}

void AGridEditor::BindToDelegate(FVector Coords) { addTile(Coords); }

void AGridEditor::DestroyTiles() {
	for (AActor* actor : tiles) {
		actor->Destroy();
	}
	tiles.Empty();
}

int AGridEditor::GetIndexOfCell(const FVector coords) {
	int indexOfCell = -1;
	UE_LOG(LogTemp, Warning, TEXT("Numero de tiles: %d"), tiles.Num());
	for (int i = 0; i < tiles.Num(); ++i) {
		UE_LOG(LogTemp, Warning, TEXT("ALOHA"));
		if (((ATile*)tiles[i])->CheckCellFree(coords)) {
			indexOfCell = i;
			break;
		}
	}
	return indexOfCell;
}

FTransform AGridEditor::GetCellTransform(const int indexOfCell) {
	return tiles[indexOfCell]->GetTransform();
}

/*
TArray<FString> GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension)
{
	// Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*directory, Visitor);
	TArray<FString> files;

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString filePath = TimestampIt.Key();
		const FString fileName = FPaths::GetCleanFilename(filePath);
		bool shouldAddFile = true;

		// Check if filename starts with required characters
		if (!onlyFilesStartingWith.IsEmpty())
		{
			const FString left = fileName.Left(onlyFilesStartingWith.Len());

			if (!(fileName.Left(onlyFilesStartingWith.Len()).Equals(onlyFilesStartingWith)))
				shouldAddFile = false;
		}

		// Check if file extension is required characters
		if (!onlyFilesWithExtension.IsEmpty())
		{
			if (!(FPaths::GetExtension(fileName, false).Equals(onlyFilesWithExtension, ESearchCase::IgnoreCase)))
				shouldAddFile = false;
		}

		// Add full path to results
		if (shouldAddFile)
		{
			files.Add(fullPath ? filePath : fileName);
		}
	}

	return files;
}
*/

/*
void AGridEditor::LoadFiles() {

	const TCHAR* path = *FPaths::Combine(FPaths::GameDir(), TEXT("Content/Map.csv"));// *(TCHAR("Content/Map.csv"));
	TArray<FString> take;

	//FFileHelper::LoadANSITextFileToStrings(path, NULL, take);
	FFileHelper::LoadFileToStringArray(take, path, FFileHelper::EHashOptions::None);
	for (int i = 0; i < take.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALOHA2"));
		FString aString = take[i];
		UE_LOG(LogTemp, Warning, TEXT("%s"), *aString);
		TArray<FString> stringArray = {};
		aString.ParseIntoArray(stringArray, TEXT(","), false);

		for (int j = 0; j < stringArray.Num(); ++j) {
			UE_LOG(LogTemp, Warning, TEXT("%s"), &stringArray[j]);
		}
	}

	/*
	std::ifstream  data(*FPaths::Combine(FPaths::GameDir(), TEXT("Content/Map.csv")));
	std::string line;
	while (std::getline(data, line))
	{

		UE_LOG(LogTemp, Warning, TEXT("ALOHA1"));

		std::stringstream  lineStream(line);
		std::string        cell;
		while (std::getline(lineStream, cell, ','))
		{
			//aString.ParseIntoArray(stringArray, TEXT(","), false);
			// You have a cell!!!!
			UE_LOG(LogTemp, Warning, TEXT("%s"), &cell);
			;
		}
	}


}
*/


