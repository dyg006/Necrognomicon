// Fill out your copyright notice in the Description page of Project Settings.

#include "BPADiscoMushroomTrap.h"
#include "Managers/NGN_DiscoMusicActorManager.h"
#include "GameMode/NGN_GameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy/NGN_GnomeController.h"


ABPADiscoMushroomTrap::ABPADiscoMushroomTrap() {
	
	PrimaryActorTick.bCanEverTick = true;

	/*EnabledTrapSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnabledSkeletalMesh"));
	EnabledTrapSkeletalMesh->SetupAttachment(SkeletalMesh);

	DisabledTrapSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisabledSkeletalMesh"));
	DisabledTrapSkeletalMesh->SetupAttachment(SkeletalMesh);*/

	danceMusicAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("danceMusicAudioComponent"));

	AreaCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
	if (AreaCollision != nullptr) {
		AreaCollision->SetBoxExtent(FVector(300.0, 300.0, 200.0));
		AreaCollision->SetupAttachment(DefaultRoot);

		FloorSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FloorSkeletalMesh"));
		if (FloorSkeletalMesh != nullptr) {
			FloorSkeletalMesh->SetupAttachment(AreaCollision);
		}
	}
	
	
}

// Called when the game starts or when spawned
void ABPADiscoMushroomTrap::BeginPlay()
{
	Super::BeginPlay();

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ANGN_DiscoMusicActorManager::StaticClass());
	discoActorManager = Cast<ANGN_DiscoMusicActorManager>(actor);
	if (discoActorManager)
	{
		discoActorManager->GetComponents(danceSoundComponents);
		danceMusicAudioComponent->SetEvent(ActivationSound);
	}

	DeactivateTrap();	
}

// Called every frame
void ABPADiscoMushroomTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Placing) {
		placingState = true;
	}
	if (placingState) {
		if (!Placing && !active) {
			ActivateTrap();
			active = true;
		}
	}
}

void ABPADiscoMushroomTrap::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsActive && DamageReceivers.Contains(OtherActor->GetClass()) && !CharmedEnemies.Contains(OtherActor)) {
		OverlapingEnemies.Add(OtherActor);
		CheckIfCanCharmEnemy();
		//UE_LOG(LogTemp, Error, TEXT("Un enemigo ha entrado en la DISCOOOSETA"));
	}
}

void ABPADiscoMushroomTrap::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (OverlapingEnemies.Contains(OtherActor))
	{
		OverlapingEnemies.Remove(OtherActor);
	}
}

void ABPADiscoMushroomTrap::CharmEnemy(AActor *enemy)
{
	if (enemy == nullptr && CharmedEnemies.Contains(enemy)) {
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("currentEnemiesDancing %d, maxNumberOfEnemies: %d"), currentEnemiesDancing, maxNumberOfEnemies);
	ANGN_GnomeController* gnomeController = Cast<ANGN_GnomeController>(Cast<APawn>(enemy)->GetController());
	if (gnomeController != nullptr) {
		gnomeController->MushroomInRadius(this, timeToDance, AreaCollision->GetUnscaledBoxExtent().X);
		CharmedEnemies.Add(enemy);
	}
}

void ABPADiscoMushroomTrap::EnemyStartedDancing() {	
	++currentEnemiesDancing;
	UE_LOG(LogTemp, Error, TEXT("currentEnemiesDancing %d, maxNumberOfEnemies: %d"), currentEnemiesDancing, maxNumberOfEnemies);
}
	

void ABPADiscoMushroomTrap::EnemyFinishedDancing() {
	--currentEnemiesDancing;
	CheckIfCanCharmEnemy();
	UE_LOG(LogTemp, Error, TEXT("currentEnemiesDancing %d, maxNumberOfEnemies: %d"), currentEnemiesDancing, maxNumberOfEnemies);
}

void ABPADiscoMushroomTrap::CheckIfCanCharmEnemy()
{
	for (AActor* enemy : OverlapingEnemies) {
		//UE_LOG(LogTemp, Error, TEXT(" Charming Enemy: %f"), timeToDance);
		if (currentEnemiesDancing < maxNumberOfEnemies) {
			CharmEnemy(enemy);
		}
		else
		{
			break;
		}
	}
}

void ABPADiscoMushroomTrap::DeactivateTrap() {
	//DisabledTrapSkeletalMesh->SetVisibility(true);
	if (SkeletalMesh != nullptr && DisabledTrapSkeletalMesh != nullptr && FloorSkeletalMesh != nullptr && danceMusicAudioComponent != nullptr) {
		danceMusicAudioComponent->SetVolume(0.f);
		FloorSkeletalMesh->SetVisibility(false);
		AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		//FloorSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
		SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		//SkeletalMesh->SetAnimation(nullptr);	
		SkeletalMesh->SetSkeletalMesh(EnabledTrapSkeletalMesh);
		BaseMaterial = SkeletalMesh->GetMaterials();
		SkeletalMesh->SetSkeletalMesh(DisabledTrapSkeletalMesh);	
		SkeletalMesh->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
		active = false;
	}
	
}

void ABPADiscoMushroomTrap::ActivateTrap() {
	if (SkeletalMesh != nullptr && EnabledTrapSkeletalMesh != nullptr && FloorSkeletalMesh != nullptr && ConstructionAnimationMushroom != nullptr && danceMusicAudioComponent != nullptr) {
		
		danceMusicAudioComponent->AddRelativeLocation(GetActorLocation());
		if (danceSoundComponents.Num() > 0)
		{
			danceMusicAudioComponent->SetTimelinePosition(Cast<UFMODAudioComponent>(danceSoundComponents[0])->GetTimelinePosition());
		}
		danceMusicAudioComponent->SetVolume(1.f);
		AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		FloorSkeletalMesh->SetVisibility(true);
		SkeletalMesh->SetSkeletalMesh(EnabledTrapSkeletalMesh);
		SkeletalMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
		SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		SkeletalMesh->SetRelativeLocation(SkeletalMesh->GetRelativeTransform().GetLocation() - FVector(0.f, 0.f, 20.f));
		//SkeletalMesh->PlayAnimation(ConstructionAnimationMushroom, false);
		//FloorSkeletalMesh->PlayAnimation(ConstructionAnimationFloor, false);
		//GetWorldTimerManager().SetTimer(TimerHandle_DiscoMushroomMesh, this, &ABPADiscoMushroomTrap::changeMeshVisibility, DelayMesh, false);
	}
}

void ABPADiscoMushroomTrap::ActivateDanceAnimation() {
	GetWorldTimerManager().ClearTimer(TimerHandle_DiscoMushroomDance);	
	
	if (DanceAnimation != nullptr) {
		FloorSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkeletalMesh->PlayAnimation(DanceAnimation, true);
		active = true;
	}
}