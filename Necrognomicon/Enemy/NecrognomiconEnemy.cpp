// Fill out your copyright notice in the Description page of Project Settings.


#include "NecrognomiconEnemy.h"
#include "Traps/BPACoalTrap.h"
#include "Traps/BPADiscoMushroomTrap.h"
#include "NGN_GnomeController.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/DamageType.h"
#include "Components/MyDamageType.h"
#include "Engine/World.h"
#include "DestructibleComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/NecrognomiconCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/DamageType.h"
#include "GameMode/NGN_GameInstance.h"
#include "GameFramework/DamageType.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Traps/BPABarrelTrap.h"
#include "DrawDebugHelpers.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ANecrognomiconEnemy::ANecrognomiconEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->Init(100);

	GetCapsuleComponent()->SetCapsuleHalfHeight(70);
	GetCapsuleComponent()->SetCapsuleRadius(50);

	// Rotate mesh to follow ArrowComponent
	FRotator NewRotation = FRotator(0.f, 0.f, -90.f);
	FQuat QuatRotation = FQuat(NewRotation);
	GetMesh()->SetWorldRotation(QuatRotation);

	//CollisionMesh->ReceiveBeginPlay();

	enemyMeshComponent = GetMesh();

	isSpeedModified = false;
	hasResidualDamage = false;
	ResidualDamageEvent.AddDynamic(this, &ANecrognomiconEnemy::StartResidualDamage);
}

// Called when the game starts or when spawned
void ANecrognomiconEnemy::BeginPlay()
{
	Super::BeginPlay();

	dynamicMaterial = UMaterialInstanceDynamic::Create(enemyMeshComponent->GetMaterial(0), nullptr);
	enemyMeshComponent->SetMaterial(0, dynamicMaterial);
	
	if (dynamicMaterial != nullptr) {
		FMaterialParameterInfo materialInfo;
		materialInfo.Name = "contrast_fire";
		dynamicMaterial->GetScalarParameterValue(materialInfo, defaultContrast_fire);
		materialInfo.Name = "contrast_ice";
		dynamicMaterial->GetScalarParameterValue(materialInfo, defaultContrast_ice);
		materialInfo.Name = "emissive_fire";
		dynamicMaterial->GetScalarParameterValue(materialInfo, defaultEmissive_fire);
		materialInfo.Name = "emissive_ice";
		dynamicMaterial->GetScalarParameterValue(materialInfo, defaultContrast_ice);
		
		materialInfo.Name = "erode";
		dynamicMaterial->GetScalarParameterValue(materialInfo, defaultErode);
		dynamicMaterial->SetScalarParameterValue("erode", currentErode);
	}

	UFMODBlueprintStatics::PlayEventAttached(laughSound, enemyMeshComponent, "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);

	OnActorHit.AddDynamic(this, &ANecrognomiconEnemy::OnHit);
}

// Called every frame
void ANecrognomiconEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If enemy has been launched, check if it should self destruct, rotate it if not
	if (HasBeenLaunched) {

		float CurrentTime = GetWorld()->GetTimeSeconds();
		if ((CurrentTime - TimeToLiveSinceLaunch) > TimeLaunched) {
			UE_LOG(LogTemp, Warning, TEXT("Enemy self destructed after launch with no collision"));
			TSubclassOf<UDamageType> DmgTypeClass = UDamageType::StaticClass();
			FHitResult Hit;
			FVector ShotDirection(0.f, 0.f, 0.f);
			FPointDamageEvent DamageEvent = FPointDamageEvent(HealthComponent->health, Hit, ShotDirection, DmgTypeClass);
			TakeDamage(HealthComponent->health, DamageEvent, this->GetInstigatorController(), ResidualDamageCauser);
			return;
		}

		FRotator NewRotation = FRotator(20.f, 0.f, 0.f);
		FQuat QuatRotation = FQuat(NewRotation);
		AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
	}

	if (currentErode > defaultErode)
	{
		currentErode -= ErodeSpeed * DeltaTime;
		currentErode = FMath::Clamp(currentErode, defaultErode, 2.0f);
		dynamicMaterial->SetScalarParameterValue("erode", currentErode);
	}

}

// Called to bind functionality to input
void ANecrognomiconEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ANecrognomiconEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	//float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamage = Damage;

	// DamageType example
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();

	// This could cause a crash if deleted
	if (dynamicMaterial == nullptr) {
		return 0.0f;
	}

	// Health component
	if (ActualDamage != 0.f)
	{

		ABPABarrelTrap* const barrel = Cast<ABPABarrelTrap>(DamageCauser);
		if (barrel && !HasBeenLaunched)
		{
			UE_LOG(LogTemp, Warning, TEXT("Daño por barril"));
			
			FVector point = GetActorLocation() - barrel->GetActorLocation();
			point.Z += 90;

			point *= 1800;
			LaunchSelf(point, DamageCauser);
		}
		else {
			
			// Apply the damage amount
			ActualDamage = HealthComponent->Damage(ActualDamage);
			
			// Sets how broken is the material
			float materialHeight = (1.0f - HealthComponent->GetHealthPercentage()) * 6.0f;
			dynamicMaterial->SetScalarParameterValue("HEIGHT", materialHeight);

			ANecrognomiconGameMode* gameMode = Cast<ANecrognomiconGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			// if dead destroy
			if (!HealthComponent->alive) {

				ANGN_GnomeController* gnomeController = Cast<ANGN_GnomeController>(GetController());
				ABPADiscoMushroomTrap* currentMushroom = gnomeController ? gnomeController->currentMushroomObjective : nullptr;

				// if controller is not null
				if (gnomeController)
				{
					//if destroyed decrease the number of gnomes dancing on the mushroom
					if (currentMushroom != nullptr) {
						//currentMushroom->CharmedEnemies.Remove(this);
						if (gnomeController->GetIsDancing()) {
							currentMushroom->EnemyFinishedDancing();
						}
					}
					gnomeController->UnregisterFromPlayerSlot();
				}

				
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), crashSound, GetActorTransform(), true);

				if (gameMode != nullptr) {
					gameMode->IncreaseScore();
				}
				// Damage event with point direction
				if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
				{
					FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
					FVector DelegateDirection = PointDamageEvent->ShotDirection;

					// Delay destroy of the actor because array indexing problems
					if (!GetWorldTimerManager().IsTimerActive(DestroyTimerHandle))
					{
						FTimerDelegate DestroyDelegate = FTimerDelegate::CreateUObject(this, &ANecrognomiconEnemy::DestroyTimer, DelegateDirection);
						GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, DestroyDelay, false);
					}

				}
				else {
					// generic death
					Destroy();
				}

				// Evento de combo

			}

			// Add killed gnome info to score manager
			if(gameMode != nullptr)
			{
				Cast<UNGN_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetScoreManager()->
					KilledGnome(this->GetClass()->GetName(), 
						HealthComponent->health, 
						ActualDamage, 
						DamageCauser->GetClass()->GetName(), 
						this->GetActorLocation(), 
						gameMode->CurrentLevel);
			}
			else // endless (no level)
			{
				Cast<UNGN_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetScoreManager()->
					KilledGnome(this->GetClass()->GetName(),
						HealthComponent->health,
						ActualDamage,
						DamageCauser->GetClass()->GetName(),
						this->GetActorLocation(),
						0);
			}
			

		}
		
		
	}

	return ActualDamage;
}

void ANecrognomiconEnemy::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	
	//If enemy has been launched, destroy on any collision
	if (HasBeenLaunched) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy collided after being launched"));
		TSubclassOf<UDamageType> DmgTypeClass = UDamageType::StaticClass();
		FHitResult Hit;
		FVector ShotDirection(0.f, 0.f, 0.f);
		FPointDamageEvent DamageEvent = FPointDamageEvent(200, Hit, ShotDirection, DmgTypeClass);
		TakeDamage(HealthComponent->health, DamageEvent, ResidualDamageCauser->GetInstigatorController(), ResidualDamageCauser);
	}
	return;
}

void ANecrognomiconEnemy::CausePainTo(AActor* enemy, bool useDistance)
{

	if ( PlayerDamage <= 0.f)
	{
		return;
	}
	if (useDistance && GetDistToEnemy(enemy) > _attackRange)
	{
		return;
	}

	//UE_LOG(LogTemp, Error, TEXT("TakeDamage by gnome %f to %s"), PlayerDamage, *enemy->GetName());
	// Create a damage event  
	//TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	//FDamageEvent DamageEvent(ValidDamageTypeClass);
	//ANecrognomiconCharacter* player = Cast<ANecrognomiconCharacter>(Player);
	// Create a damage event  
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	AController* DamageInstigator = GetInstigatorController();
	enemy->TakeDamage(PlayerDamage, DamageEvent, DamageInstigator, this);

}

float ANecrognomiconEnemy::GetDistToEnemy(AActor* enemy) const
{
	float distance = FVector::Dist(enemy->GetActorLocation(), GetActorLocation());
	distance -= Cast<ANecrognomiconCharacter>(enemy)->GetCapsuleComponent()->GetScaledCapsuleRadius();
	return distance;
}

void ANecrognomiconEnemy::DestroyTimer(FVector Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage CPP Direction: %s"), *Direction.ToString());

	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);

	// Calculate spawn transform
	FTransform SpawnTransform(GetMesh()->GetComponentToWorld());
	FVector DestructibleLocation = SpawnTransform.GetLocation() + SpawnOffset;
	SpawnTransform.SetLocation(DestructibleLocation);
	// Spawn destructible mesh
	ADestructibleActor* DestructibleActor = GetWorld()->SpawnActor<ADestructibleActor>(DestructibleActorClass, SpawnTransform);
	// Calculate impulse direction
	//FVector ImpulseDirection = Direction + FVector(0.f, 0.f, 80.f);
	FVector ImpulseDirection = Direction;

	//Obtenemos el ImpactPoint; no podemos sacarlo directamente de la location de un SceneComponent porque plugin ta bugao
	FVector ImpactPoint = DestructibleActor->GetActorRotation().RotateVector(ImpactPointOffset) + DestructibleActor->GetActorLocation();


	//Debugging de por que demonios siempre toma la posicion del destruction point como 0,0,0
	//DrawDebugSphere(GetWorld(), ImpactPoint, 5.f, 10, FColor::Yellow, false, 20.f, (uint8)'\000', 5.f);
	//DrawDebugSphere(GetWorld(), DestructibleActor->FindComponentByClass<USceneComponent>()->GetComponentTransform().GetLocation(), 5.f, 10, FColor::Red, false, 20.f, (uint8)'\000', 5.f);

	// Apply damage and destroy
	DestructibleActor->GetDestructibleComponent()->ApplyDamage(100.f, ImpactPoint, ImpulseDirection, Impulse);

	Destroy();
}

void ANecrognomiconEnemy::Attack( AActor* objective) {
	
	if (enemyMeshComponent != nullptr) {
		enemyAnimationInstance = enemyMeshComponent->GetAnimInstance();

		if (!enemyAnimationInstance->Montage_IsPlaying(attackAnimMontage)) {
			attackObjective = objective;
			enemyAnimationInstance->Montage_Play(attackAnimMontage);
		}
	}
}

void ANecrognomiconEnemy::StartDancing() {
	StartDanceAnimation();
}

void ANecrognomiconEnemy::StopDancing() {
	StopDanceAnimation();
}


/*
	Control daño residual
*/
void ANecrognomiconEnemy::StartResidualDamage(float Damage, float MotionModifier, float ActiveTime, AActor* DamageCauser)
{
	if (!hasResidualDamage)
	{
		ResidualDamageAmount = Damage;
		SpeedModifier = MotionModifier;
		ResidualDamageCauser = DamageCauser;

		hasResidualDamage = true;

		//Being damaged by a coal trap
		if (ResidualDamageCauser->GetClass()->GetName().Contains("Coal")) {
			dynamicMaterial->SetScalarParameterValue("emissive_fire", activeEmissive_fire);
			dynamicMaterial->SetScalarParameterValue("contrast_fire", 0.0f);
			dynamicMaterial->SetScalarParameterValue("emissive_ice", defaultEmissive_ice);
			dynamicMaterial->SetScalarParameterValue("contrast_ice", defaultContrast_ice);
		}
		else { //Being damaged by an ice trap
			dynamicMaterial->SetScalarParameterValue("emissive_ice", activeEmissive_ice);
			dynamicMaterial->SetScalarParameterValue("contrast_ice", 0.0f);
			dynamicMaterial->SetScalarParameterValue("emissive_fire", defaultEmissive_fire);
			dynamicMaterial->SetScalarParameterValue("contrast_fire", defaultContrast_fire);
		}

		UE_LOG(LogTemp, Error, TEXT("DamageCauser class: %s ABPACoalTrap::StaticClass()): %s "), *DamageCauser->GetClass()->GetName(), *ABPACoalTrap::StaticClass()->GetName());

		if (SpeedModifier != 0 && !isSpeedModified)
		{
			GetCharacterMovement()->MaxWalkSpeed *= SpeedModifier;
			isSpeedModified = true;
		}

		GetWorldTimerManager().SetTimer(TimerHandle_ResidualDamageTimer, this, &ANecrognomiconEnemy::StopResidualDamage, ActiveTime, false);

		if (!GetWorldTimerManager().IsTimerActive(TimerHandle_ReceiveDamageTimer))
			GetWorldTimerManager().SetTimer(TimerHandle_ReceiveDamageTimer, this, &ANecrognomiconEnemy::MakeResidualDamage, 1.f, true);
	}
}

void ANecrognomiconEnemy::StopResidualDamage()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ReceiveDamageTimer);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResidualDamageTimer);
	if (SpeedModifier != 0)
	{
		GetCharacterMovement()->MaxWalkSpeed /= SpeedModifier;
		isSpeedModified = false;
	}

	dynamicMaterial->SetScalarParameterValue("emissive_fire", defaultEmissive_fire);
	dynamicMaterial->SetScalarParameterValue("contrast_fire", defaultContrast_fire);
	dynamicMaterial->SetScalarParameterValue("emissive_ice", defaultEmissive_ice);
	dynamicMaterial->SetScalarParameterValue("contrast_ice", defaultContrast_ice);

	hasResidualDamage = false;
}

void ANecrognomiconEnemy::MakeResidualDamage()
{	
	if (ResidualDamageAmount > 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("TakeDamage by trap: %f"), ResidualDamageAmount);
		TSubclassOf<UDamageType> DmgTypeClass = nullptr;
		FHitResult Hit;
		FVector ShotDirection(0.f, 0.f, 1.f);
		FPointDamageEvent DamageEvent = FPointDamageEvent(ResidualDamageAmount, Hit, ShotDirection, DmgTypeClass);
		TakeDamage(ResidualDamageAmount, DamageEvent, this->GetInstigatorController(), ResidualDamageCauser);
	}
}

void ANecrognomiconEnemy::LaunchSelf(FVector ImpulseVector, AActor* DamageCauser) {
	//Ignore collision with other enemies, start simulating physics,
	//add given impulse, set HasBeenLaunched to true, deactivate AI is controller exists
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	Capsule->SetSimulatePhysics(true);
	Capsule->AddImpulse(ImpulseVector);
	HasBeenLaunched = true;
	ResidualDamageCauser = DamageCauser;
	ANGN_GnomeController* GnomeController = Cast<ANGN_GnomeController>(GetController());
	if (GnomeController) { //only deactivate AI if it is an active enemy
		GnomeController->SetAIActive(false);
	}

	//Set launch time
	TimeLaunched = GetWorld()->GetTimeSeconds();

	return;
}

void ANecrognomiconEnemy::JumpForward(float Distance, float Duration)
{
	FVector origin = GetActorLocation();
	FVector destination = GetActorLocation() +  GetActorForwardVector() * Distance;

	float ZOrigin = origin.Z + ( FMath::Square(Duration) * -0.5f * 982 );
	origin.Z = ZOrigin;

	FVector LaunchVelocity = (destination - origin) / Duration;
	this->LaunchCharacter(LaunchVelocity, true, true);
}

void ANecrognomiconEnemy::Reset()
{
	// We have different types of gnomes: normal, endless, tooltip
	// We can not destroy every single gnome anymore.
	if (Controller != nullptr)
	{
		Destroy();
	}
}