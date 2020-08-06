// Fill out your copyright notice in the Description page of Project Settings.


#include "BPAObject.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Traps/BPATrap.h"
#include "Camera/PlayerCameraManager.h"
#include "DrawDebugHelpers.h"
#include "Math/Color.h"

// Sets default values
ABPAObject::ABPAObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	Tooltip = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Tooltip->SetupAttachment(DefaultRoot);

	HightlightDistance = 230.f;
	//PlayerOverlapEvent.AddDynamic(this, &ABPAObject::ChangeHiglightStatus);

	Tooltip->SetVisibility(false);
	Tooltip->SetBlendMode(EWidgetBlendMode::Transparent);
	Placing = false;
	CanBePlaced = true;
}

// Called when the game starts or when spawned
void ABPAObject::BeginPlay()
{
	Super::BeginPlay();

	Tooltip->SetVisibility(false);
	
}

// Called every frame
void ABPAObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABPAObject::StartFocus()
{
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector CameraLocation = CameraManager->GetCameraLocation();
	//FVector CameraForwardVector = CameraManager->GetCameraRotation().Vector();
	//UE_LOG(LogTemp, Warning, TEXT("Location: %f, %f, %f"), CameraLocation.X, CameraLocation.Y, CameraLocation.Z);

	Tooltip->SetVisibility(true);
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), CameraLocation);
	Tooltip->SetWorldRotation(Rotation);

	this->PlayerOverlapEvent.Broadcast(true);
	
}

void ABPAObject::EndFocus()
{
	Tooltip->SetVisibility(false);
	this->PlayerOverlapEvent.Broadcast(false);

}

bool ABPAObject::UsarObjeto()
{
	Placing = true;
	CanBePlaced = false;
	Tooltip->SetVisibility(false);
	return true;
}
