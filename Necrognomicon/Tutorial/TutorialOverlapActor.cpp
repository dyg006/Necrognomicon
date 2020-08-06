// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialOverlapActor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/NecrognomiconCharacter.h"


// Sets default values
ATutorialOverlapActor::ATutorialOverlapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	Tooltip = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Tooltip->SetupAttachment(DefaultRoot);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetBoxExtent(FVector(32.0, 32.0, 32.0));
	BoxCollision->SetupAttachment(DefaultRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(DefaultRoot);

	Tooltip->SetVisibility(false);
	Tooltip->SetBlendMode(EWidgetBlendMode::Transparent);
}

// Called when the game starts or when spawned
void ATutorialOverlapActor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialOverlapActor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ATutorialOverlapActor::OnOverlapEnd);

	Tooltip->SetVisibility(false);
	
}

// Called every frame
void ATutorialOverlapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPlayerOverlaping)
	{
		CheckPlayerOverlap();
	}
	else
	{
		EndFocus();
	}
}


void ATutorialOverlapActor::CheckPlayerOverlap()
{
	
	float dotProduct = _necroCharacter->GetDotProductTo(this);
	if (_necroCharacter != nullptr && dotProduct >= 0.0f)
		StartFocus();
	else
		EndFocus();
}

void ATutorialOverlapActor::StartFocus()
{
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector CameraLocation = CameraManager->GetCameraLocation();

	Tooltip->SetVisibility(true);
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), CameraLocation);
	Tooltip->SetWorldRotation(Rotation);
}

void ATutorialOverlapActor::EndFocus()
{
	Tooltip->SetVisibility(false);
}


void ATutorialOverlapActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		IsPlayerOverlaping = true;
		_necroCharacter = Cast<ANecrognomiconCharacter>(OtherActor);
	}
}

void ATutorialOverlapActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player") && _necroCharacter != nullptr)
	{
		IsPlayerOverlaping = false;
		_necroCharacter = nullptr;
	}
}



