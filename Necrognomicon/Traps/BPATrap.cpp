// Fill out your copyright notice in the Description page of Project Settings.

#include "BPATrap.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "GameMode/EndlessGameMode.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

// Sets default values
ABPATrap::ABPATrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = DefaultRoot;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetBoxExtent(FVector(32.0, 32.0, 32.0));
	BoxCollision->SetupAttachment(DefaultRoot);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(BoxCollision);

	// Stencil Value of 3 for trap highlight-
	SkeletalMesh->SetCustomDepthStencilValue(3);

	HIGHLIGHT_DISTANCE = 230.0f;

	Placing = false;
	CanBePlaced = true;
}

// Called when the game starts or when spawned
void ABPATrap::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMesh->SetCustomDepthStencilValue(4);
	SkeletalMesh->SetRenderCustomDepth(true);

	// Save the original materials
	BaseMaterial = SkeletalMesh->GetMaterials();

	/*
		ObjectTypeQuery1 // world static
		ObjectTypeQuery2 // world dynamic
		ObjectTypeQuery3 // pawn
		ObjectTypeQuery4 // physics body
		ObjectTypeQuery5 // vehicle
		ObjectTypeQuery6 // destructible
		ObjectTypeQuery7 // custom 1
		ObjectTypeQuery8 // Grid
	*/
	TraceGridType.Add(EObjectTypeQuery::ObjectTypeQuery8);

	gameMode = Cast<ANecrognomiconGameMode>(GetWorld()->GetAuthGameMode());
	player = Cast<ANecrognomiconCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABPATrap::OnOverlapBegin);
	PlayerOverlapEvent.AddDynamic(this, &ABPATrap::ChangeFocusStatus);

	ResetTransform = GetActorTransform();

	// Autoplace traps functionality on beginplay
	if (Grid) {
		if(!GetWorld()->GetTimerManager().IsTimerActive(AutoPlaceTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(AutoPlaceTimerHandle, this, &ABPATrap::AutoPlaceObjectCpp, 0.1f);
		}
	}
}

void ABPATrap::AutoPlaceObjectCpp() 
{
	// Move trap to grid location
	bool valid = false;
	float locationX, locationY;
	Grid->TileToGridLocationEvent(Row, Column, valid, locationX, locationY);
	
	if (valid)
	{
		FVector newLocation(locationX, locationY, GetActorLocation().Z);
		this->SetActorLocation(newLocation);
		
		// Place trap
		CanBePlaced = true;
		Execute_Implementation();
	}
}

void ABPATrap::ChangeFocusStatus(bool IsPlayerOverlap)
{
	if (IsPlayerOverlap)
		BeginFocus();

	else
		EndFocus();
}

void ABPATrap::BeginFocus()
{
	if (OverGrid) {
		SkeletalMesh->SetRenderCustomDepth(true);
		SkeletalMesh->SetCustomDepthStencilValue(3);
		
		if (gameMode) {
			if (gameMode->IsLevelHordeActive)
			{
				Tooltip->SetVisibility(false);
			}
		}
		else
		{
			Tooltip->SetVisibility(false);
		}

	}
}

void ABPATrap::EndFocus()
{
	if (OverGrid)
		SkeletalMesh->SetRenderCustomDepth(false);
	else
		SkeletalMesh->SetCustomDepthStencilValue(4);
}

// Called every frame
void ABPATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Placing)
	{
		UpdatePlacingPositionCpp();
	}
}

void ABPATrap::action()
{
}

void ABPATrap::PlaceObjectCpp()
{
	this->IsInInventory = false;
	EndFocus();
	ABPAObject::EndFocus();
	// Restore original mesh
	for (int i = 0; i < BaseMaterial.Num(); ++i) {
		SkeletalMesh->SetMaterial(i, BaseMaterial[i]);
	}
	// Mark tile as occupied
	Grid->SetFreeTileEvent(Row, Column, false);
	// Activate collisions
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// Stop placing logic
	Placing = false;

	OverGrid = true;
	SkeletalMesh->SetRenderCustomDepth(false);
}

void ABPATrap::UpdatePlacingPositionCpp()
{
	OverGrid = false;
	Tooltip->SetVisibility(false);
	SkeletalMesh->SetRenderCustomDepth(true);
	SkeletalMesh->SetCustomDepthStencilValue(4);

	// Raycast trying to search grids
	FVector cameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector forwardVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();

	FVector start = cameraLocation + forwardVector * 200;
	FVector end = cameraLocation + forwardVector * 5000;

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByObjectType(HitResult, start, end, FCollisionObjectQueryParams(TraceGridType)))
	{
		FVector hitLocation = HitResult.Location;
		Grid = Cast<ANGN_Grid>(HitResult.GetActor());

		bool valid = false;
		FVector2D gridLocation;
		Grid->LocationToGridLocationEvent(hitLocation, true, valid, gridLocation, Row, Column);
		if (valid) {
			bool free = false;
			Grid->IsFreeTileEvent(Row, Column, free);
			if (free) {
				this->SetActorLocation(FVector(gridLocation.X, gridLocation.Y, hitLocation.Z));
				this->SetActorRotation(Grid->GetTransform().GetRotation());

				ABPAObject::EndFocus();
				for (int i = 0; i < BaseMaterial.Num(); ++i) {
					SkeletalMesh->SetMaterial(i, GreenMaterial);
				}
				this->SetActorHiddenInGame(false);

				// Hide trap location widget
				if (gameMode != nullptr) {
					gameMode->SetVisibilityTrapPlacementWidget(false);
				}
				else {
					Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode())->SetVisibilityTrapPlacementWidget(false);
				}

				// Disable collision so we dont have problems with gnomes
				SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				CanBePlaced = true;
			}
			else {
				InvalidGridPosition();
			}
		}
		else {
			InvalidGridPosition();
		}
	}
	else {
		InvalidGridPosition();
	}
}

void ABPATrap::InvalidGridPosition()
{
	this->SetActorHiddenInGame(true);
	// Deactivate collisions
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Deactivate trap placement widget
	if (gameMode != nullptr) {
		gameMode->SetVisibilityTrapPlacementWidget(true);
	}
	else {
		Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode())->SetVisibilityTrapPlacementWidget(true);
	}

	CanBePlaced = false;
}

void ABPATrap::Select_Implementation()
{
	FVector forward = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorForwardVector() * 100;
	FTransform transform = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorTransform();
	FTransform trapTransform = FTransform(transform.GetRotation(), forward + transform.GetLocation(), transform.GetScale3D());
	SetActorTransform(trapTransform);
	// Green material
	ABPAObject::EndFocus();
	for (int i = 0; i < BaseMaterial.Num(); ++i) {
		SkeletalMesh->SetMaterial(i, GreenMaterial);
	}
	if (player != NULL) {
		if (player->GetMesh() != nullptr && BaseRenAnimationBlueprint != nullptr)
			player->GetMesh()->SetAnimInstanceClass(BaseRenAnimationBlueprint);
	}
	this->UsarObjeto();
}

void ABPATrap::Execute_Implementation()
{
	if (CanBePlaced)
	{
		PlaceObjectCpp();
		if (ConstructionAnimation != nullptr) {
			SkeletalMesh->PlayAnimation(ConstructionAnimation, false);
		}
		if (ConstructionSound != nullptr) {
			UFMODBlueprintStatics::PlayEventAttached(ConstructionSound, SkeletalMesh, "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
		}
	}
	else {
		if (player != nullptr) {
			player->PlayErrorSound();
		}
	}
}

// Pick up the object
void ABPATrap::Interaction_Implementation()
{
	IsActive = false;
	this->IsInInventory = true;
	if (player)
	{
		bool anadido = false;
		player->AnadirAInventario(EstructuraObjeto, 1, 0, anadido);
		if (anadido)
		{
			player->ActualizarInventario();
			// Free the tile
			if (Grid)
			{
				Grid->SetFreeTileEvent(Row, Column, true);
			}
			this->Destroy();
		}
	}
}

void ABPATrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActivationActors.Contains(OtherActor->GetClass()))
		action();
}

void ABPATrap::Reset()
{
}