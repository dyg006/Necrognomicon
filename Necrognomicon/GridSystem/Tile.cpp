// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile() :heightLevel(0), sideSize(100), coords(FVector()), rotateHorizontal(0.f), rotateVertical(0.f)
, fixedUp(false), fixedDown(false), fixedLeft(false), fixedRight(false)
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::Init(float size, FVector coords,
	UMaterialInterface* baseMaterial, UMaterialInterface* roadMaterial, UMaterialInterface* houseMaterial, UMaterialInterface* deleteMaterial
	//,	AGridEditor* editor
) {
	//this->editor = editor;
	this->sideSize = size;
	this->coords = coords;
	this->baseMaterial = UMaterialInstanceDynamic::Create(baseMaterial, mesh);
	this->roadMaterial = UMaterialInstanceDynamic::Create(roadMaterial, mesh);
	this->houseMaterial = UMaterialInstanceDynamic::Create(houseMaterial, mesh);
	this->deleteMaterial = UMaterialInstanceDynamic::Create(deleteMaterial, mesh);
	//set paramater with Set***ParamaterValue
	/*this->baseMaterial->SetScalarParameterValue("MyParameter", 1.0);
	this->roadMaterial->SetScalarParameterValue("MyParameter", 1.0);
	this->houseMaterial->SetScalarParameterValue("MyParameter", 1.0);
	this->deleteMaterial->SetScalarParameterValue("MyParameter", 1.0);*/

	// Crear malla procedural del tamaño elegido
	_vertices_base = {
		FVector(0.f, 0.f, 0.f),
		FVector(size, 0.f, 0.f),
		FVector(0.f, size, 0.f),
		FVector(size, size, 0.f)
	};
	_vertices = _vertices_base;
	setMesh(_vertices, false);

	// Crear texto de coordenadas
	setText(coords.Y, coords.X, coords.Z);

	// Asignar material a la malla;
	setMaterial();
}

FVector ATile::computeNormal(const FVector p1, const FVector p2, const FVector p3) const {
	FVector V = p2 - p1;
	FVector W = p3 - p1;
	return {
		(V.Y * W.Z) - (V.Z * W.Y),
		(V.Z * W.X) - (V.X * W.Z),
		(V.X * W.Y) - (V.Y * W.X)
	};
}

void ATile::setMaterial() {
	UMaterialInstanceDynamic* newMaterial;
	switch (type) {
	case DELETE: newMaterial = deleteMaterial;
		break;
	case ROAD: newMaterial = roadMaterial;
		break;
	case HOUSE: newMaterial = houseMaterial;
		break;
	default: newMaterial = baseMaterial;
		break;
	}
	if (newMaterial && mesh) {
		mesh->SetMaterial(0, newMaterial);
	}
}

void ATile::setText(const int x, const int y, const int z) {
	if (!text) {
		text = NewObject<UTextRenderComponent>(this);
		text->SetRelativeLocation(FVector(.1f, .1f, .01f));
		text->SetRelativeRotation(FRotator(90.0f, 180.0f, .0f));
		text->SetTextRenderColor(FColor::White);
		text->SetXScale(.1f);
		text->SetYScale(.1f);
		text->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		text->RegisterComponent();
	}
	text->SetText(FString::Printf(TEXT("(%i,%i,%i)"), y, x, z));
}

void ATile::setMesh(TArray<FVector> vertices, bool update) {
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	TArray<int32> _triangles = { 0,2,1,1,2,3 }; //Generamos los triangulos asociados
	for (int i = 0; i < 2; ++i) {
		normals.Add(computeNormal(vertices[_triangles[i * 3]], vertices[_triangles[i * 3 + 1]], vertices[_triangles[i * 3 + 2]]));
		UV0.Add(FVector2D(0, 0));
		tangents.Add(FProcMeshTangent(0, 1, 0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}
	if (!update) {
		mesh->CreateMeshSection_LinearColor(0, vertices, _triangles, normals, UV0, vertexColors, tangents, true);
		mesh->ContainsPhysicsTriMeshData(true); // Enable collision data
	}
	else {
		mesh->UpdateMeshSection_LinearColor(0, vertices, normals, UV0, vertexColors, tangents);
	}
}

FVector rotatePointAround(FVector point, FVector anchor, int axis, float degrees) {
	float cosAlpha = cos(degrees * (PI / 180.0));
	float sinAlpha = sin(degrees * (PI / 180.0));
	float x1 = point[axis];
	float y1 = point.Z;
	float x0 = anchor[axis];
	float y0 = anchor.Z;
	float x2 = cosAlpha * (x1 - x0) - sinAlpha * (y1 - y0) + x0;
	float y2 = sinAlpha * (x1 - x0) + cosAlpha * (y1 - y0) + y0;
	if (axis == 0) return FVector(x2, anchor[1], y2) - anchor;
	else return FVector(anchor[0], x2, y2) - anchor;
}

void ATile::rotateMesh(TileRotationType rotationType) {
	_vertices = _vertices_base;
	FVector rotation;

	//Calcular nuevas posiciones
	switch (rotationType) {
	case TileRotationType::ROTATE_HORIZONTAL:
		rotateVertical = 0;
		if (rotateHorizontal > 0) {
			rotation = rotatePointAround(_vertices[2], _vertices[0], 1, rotateHorizontal);
			_vertices[2] = _vertices[0] + rotation;
			_vertices[3] = _vertices[1] + rotation;

			//text->SetRelativeRotation(FRotator(90.0f, 180.0f, rotateHorizontal));
		}
		else {
			rotation = rotatePointAround(_vertices[0], _vertices[2], 1, rotateHorizontal);
			_vertices[0] = _vertices[2] + rotation;
			_vertices[1] = _vertices[3] + rotation;
		}
		break;
	case TileRotationType::ROTATE_VERTICAL:
		rotateHorizontal = 0;
		if (rotateVertical > 0) {
			rotation = rotatePointAround(_vertices[1], _vertices[0], 0, rotateVertical);
			_vertices[1] = _vertices[0] + rotation;
			_vertices[3] = _vertices[2] + rotation;
		}
		else {
			rotation = rotatePointAround(_vertices[0], _vertices[1], 0, rotateVertical);
			_vertices[0] = _vertices[1] + rotation;
			_vertices[2] = _vertices[3] + rotation;
		}
		break;
	}
	//Actualizar malla
	setMesh(_vertices, true);

	//Avisar a tiles adjacentes si está magnetizado
}

void ATile::addTileUp() const { addTile(TileSide::UP_SIDE); }
void ATile::addTileDown() const { addTile(TileSide::DOWN_SIDE); }
void ATile::addTileLeft() const { addTile(TileSide::LEFT_SIDE); }
void ATile::addTileRight() const { addTile(TileSide::RIGHT_SIDE); }
void ATile::addTile(TileSide side) const {
	//Pasar coordenadas de grid, no coordenadas del mundo
	FVector v = coords;
	switch (side) {
	case TileSide::LEFT_SIDE: v += _vertices[0]; v.Y -= sideSize; break;
	case TileSide::RIGHT_SIDE: v += _vertices[2]; break;
	case TileSide::DOWN_SIDE: v += _vertices[0]; v.X -= sideSize; break;
	case TileSide::UP_SIDE: v += _vertices[1]; break;
	}
	//Broadcast delegate
	AddTileOnDelegate.Broadcast(v);
}


bool ATile::CheckCellFree(const FVector objCoords) {
	//bool coordsContained = false;
	//if
	//UE_LOG(LogTemp, Warning, TEXT("(%f,%f,%f,%f), (%f,%f,%f)"), this->coords.X, this->coords.Y, this->coords.Z, sideSize, objCoords.X, objCoords.Y, objCoords.Z);
	return (objCoords.X >= this->coords.X
		&& objCoords.X <= this->coords.X + sideSize
		&& objCoords.Y >= this->coords.Y
		&& objCoords.Y <= this->coords.Y + sideSize);
	

	/*
	return (coords.X >= this->coords.X
		&& coords.X <= this->coords.X + _vertices[3].X
		&& coords.Y >= this->coords.Y
		&& coords.Y <= this->coords.Y + _vertices[3].Y);
	*/
}


#if WITH_EDITOR  
void ATile::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

	//Get all of our components  
	TArray<UActorComponent*> MyComponents;
	GetComponents(MyComponents);

	//Get the name of the property that was changed  
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// We test using GET_MEMBER_NAME_CHECKED so that if someone changes the property name  
	// in the future this will fail to compile and we can update it.  
	//if ((PropertyName == GET_MEMBER_NAME_CHECKED(AGridEditor, MyMesh)))ç
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(ATile, type))) { setMaterial(); }
	else if ((PropertyName == GET_MEMBER_NAME_CHECKED(ATile, rotateHorizontal))) { rotateMesh(TileRotationType::ROTATE_HORIZONTAL); }
	else if ((PropertyName == GET_MEMBER_NAME_CHECKED(ATile, rotateVertical))) { rotateMesh(TileRotationType::ROTATE_VERTICAL); }

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif