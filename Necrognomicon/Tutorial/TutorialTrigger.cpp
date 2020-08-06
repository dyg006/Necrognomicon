// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameMode/NGN_GameInstance.h"
#include "Character/NecrognomiconCharacter.h"


// Sets default values
ATutorialTrigger::ATutorialTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = DefaultRoot;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetBoxExtent(FVector(32.0, 32.0, 32.0));
	BoxCollision->SetupAttachment(DefaultRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxCollision);

	//_gameInstance = Cast<UNGN_GameInstance>(GetGameInstance());
}

// Called when the game starts or when spawned
void ATutorialTrigger::BeginPlay()
{

	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialTrigger::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ATutorialTrigger::OnOverlapEnd);

	_rotator = FRotator(Pitch_Y, Yaw_Z, Roll_X);

}

// Called every frame
void ATutorialTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRotationEnabled)
	{
		Mesh->AddWorldRotation(_rotator);
	}

}



void ATutorialTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
}

void ATutorialTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ANecrognomiconCharacter>(OtherActor) != nullptr)
	{
		Destroy();
	}
}

