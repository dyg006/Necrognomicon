// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecrognomiconCharacter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Camera/CameraComponent.h"
#include "Components/SlotManagerComponent.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Inventory/BPAObject.h"
#include "Enemy/NecrognomiconEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/EndlessGameMode.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "NecrognomiconController.h"


//////////////////////////////////////////////////////////////////////////
// ANecrognomiconCharacter

ANecrognomiconCharacter::ANecrognomiconCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	healthComponent->defaultHealth = 100;
	healthComponent->alive = true;

	HeartbeatSound = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("HeartbeatSound"));
	HeartbeatSound->SetupAttachment(RootComponent);

	errorAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("errorAudioComponent"));
	errorAudioComponent->SetupAttachment(RootComponent);
	//ConstructorHelpers::FObjectFinder<UActorComponent> inventory(TEXT("'/Game/AA_GYM/Inventory/Necrognomicon_Assets/BPC_Inventario.BPC_Inventario'"));

	//playerInventory = inventory.Object;

	playerMeshComponent = GetMesh();	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	slotManager = CreateDefaultSubobject<USlotManagerComponent>(TEXT("SlotManagerComponent"));
	
	slotManager->SetupAttachment(RootComponent);
	slotManager->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	slotManager->SetWorldLocation(GetActorLocation());
}

void ANecrognomiconCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UpdateHeartbeatSound();

	gameMode = Cast<ANecrognomiconGameMode>(GetWorld()->GetAuthGameMode());

	endlessGameMode = Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode());

	dynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	GetMesh()->SetMaterial(0, dynamicMaterial);

	secondaryDynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), nullptr);
	GetMesh()->SetMaterial(1, secondaryDynamicMaterial);

	CameraRotation = FollowCamera->RelativeRotation;
	CameraLocation = FollowCamera->RelativeLocation;
}

void ANecrognomiconCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerDeath(DeltaTime);

	// COMENTADO POR EL MOMENTO PARA QUE PABLITO CONTINUE POR AQUI
	CheckCameraCollision(DeltaTime);

	CheckPickableObject();
}

void ANecrognomiconCharacter::CheckPlayerDeath(float DeltaTime)
{
	if (gameMode != nullptr)
	{
		// Check if player is dead and wait timeBeforeRespawn seconds to respawn
		if (gameMode->GetIsPlayerDead()) {
			timeWaitingForRespawn += DeltaTime;

			if (collisionEnabled) { // if player is dead and collision is enabled we disable his collisions
				DisableCollisionsAndInput();
				collisionEnabled = false;
			}

			if (timeWaitingForRespawn >= timeBeforeRespawn) {// if the player has died and the time before respawn has passed we enable collisions again and respawn the player
				if (!collisionEnabled) {
					EnableCollisionsAndInput();
					RespawnPlayer();
					collisionEnabled = true;
					gameMode->SetIsPlayerDead(false);
				}
			}
		}
		else {
			CheckIfCanRestoreHealth(DeltaTime);
		}

		if (isPlayerInvulnerable) { // Calculate the time the player is invulnerable
			timeBeingInvulnerable += DeltaTime;
			blinkingTimer += DeltaTime;
			if (blinkingTimer >= blinkingDelay) {
				blinkingTimer = 0;
				StartBlinking();
			}
			
			if (timeBeingInvulnerable >= invincibleTime) {
				StopBlinking();
				isPlayerInvulnerable = false;
			}
		}
	}
}

void ANecrognomiconCharacter::DisableCollisionsAndInput() {

	GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
	/* Enable ragdolls */
	/*GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->AddImpulse(FVector(0, 10000, 0));*/
	/********************/
	//GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	collisionEnabled = false;

}

void ANecrognomiconCharacter::EnableCollisionsAndInput() {

	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
	/* Disable ragdolls */
	/*GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");*/
	/********************/
	//GetCharacterMovement()->Activate();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	collisionEnabled = true;
}

void ANecrognomiconCharacter::CheckPickableObject()
{
	TSet<AActor*> pickList;
	TSet<ABPAObject*> pickListInsideCustomDistance;
	GetOverlappingActors(pickList, ABPAObject::StaticClass());

	if (pickList.Num() > 0)
	{
		for (AActor* const Object : pickList)
		{
			float currentDistance = Object->GetDistanceTo(this);
			ABPAObject* currentItem = Cast<ABPAObject>(Object);
			if (currentDistance < currentItem->HightlightDistance && !currentItem->IsInInventory)
			{

				pickListInsideCustomDistance.Add(currentItem);
			}
			else {
				if (pickableItem != nullptr)
				{
					pickableItem->EndFocus();
					pickableItem = nullptr;
					previousDistance = 0.f;
				}
			}
		}


		for (ABPAObject* const Object : pickListInsideCustomDistance)
		{

			FVector playerLocation = this->GetActorLocation();
			FVector objectLocation = Object->GetActorLocation();
			float dotProduct = this->GetDotProductTo(Object);

			if (dotProduct >= 0.0f)
			{
				if (pickableItem != Object && dotProduct > previousDistance)
				{
					if (pickableItem != nullptr)
						pickableItem->EndFocus();

					pickableItem = Object;
					pickableItem->StartFocus();

					previousDistance = dotProduct;
				}

				if (pickableItem == Object)
				{
					pickableItem->StartFocus();
					previousDistance = dotProduct;
				}
			}

			else if (dotProduct < 0.2f && pickableItem == Object)
			{
				UE_LOG(LogTemp, Warning, TEXT("currentItem: %s"), *Object->GetName());

				pickableItem->EndFocus();
				pickableItem = nullptr;
				previousDistance = 0.f;
			}

		}



	}
	
	else {
		if (pickableItem != nullptr)
		{
			pickableItem->EndFocus();
			pickableItem = nullptr;
			previousDistance = 0.f;
		}
	}
	
}

void ANecrognomiconCharacter::RespawnPlayer() {
	if (gameMode != nullptr) {
		gameMode->RespawnPlayer();

		timeBeingInvulnerable = 0.f;
		isPlayerInvulnerable = true;

		healthComponent->alive = true;
		healthComponent->health = healthComponent->defaultHealth;
		this->SpawnEvent.Broadcast();
	}
}

void ANecrognomiconCharacter::Reset()
{
	UE_LOG(LogTemp, Warning, TEXT("Character Reset custom"));

	timeWaitingForRespawn = timeBeforeRespawn;
	if(gameMode != nullptr)
	{
		gameMode->SetIsPlayerDead(true);
	}
	
}

void ANecrognomiconCharacter::ActivateReloadAnimation() {
	if (playerMeshComponent != nullptr) {
		playerAnimation = playerMeshComponent->GetAnimInstance();
	}
	if (firingAnimMontage != nullptr && playerAnimation != nullptr) {
		if (!playerAnimation->Montage_IsPlaying(reloadMontage)) {
			playerAnimation->Montage_Play(reloadMontage);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error trying to load firing anim Montage"));
	}
}

void ANecrognomiconCharacter::StopReloadAnimation() {
	if (playerMeshComponent != nullptr) {
		playerAnimation = playerMeshComponent->GetAnimInstance();
	}
	if (firingAnimMontage != nullptr && playerAnimation != nullptr) {
		if (!playerAnimation->Montage_IsPlaying(firingAnimMontage)) {
			//playerAnimation->Montage_Stop( 1.0f, firingAnimMontage);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error trying to load firing anim Montage"));
	}
}

void ANecrognomiconCharacter::ActivateFireAnimation() {
	UE_LOG(LogTemp, Error, TEXT("STARTING TO ATTACK"));
	//if (attackIsReady) {
		UE_LOG(LogTemp, Error, TEXT("ATTACK IS READY"));
		if (playerMeshComponent != nullptr) {
			playerAnimation = playerMeshComponent->GetAnimInstance();
		}
		if (firingAnimMontage != nullptr && playerAnimation != nullptr) {
			if (!playerAnimation->Montage_IsPlaying(firingAnimMontage)) {
				playerAnimation->Montage_Play(firingAnimMontage);
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Error trying to load firing anim Montage"));
		}
	//}
}

void ANecrognomiconCharacter::stopFireAnimation() {
	if (playerMeshComponent != nullptr) {
		playerAnimation = playerMeshComponent->GetAnimInstance();
	}
	if (firingAnimMontage != nullptr && playerAnimation != nullptr) {
		if (playerAnimation->Montage_IsPlaying(firingAnimMontage)) {
			//playerAnimation->Montage_Stop( 1.0f, firingAnimMontage);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error trying to load firing anim Montage"));
	}
}

void ANecrognomiconCharacter::ActivateMeleeAnimation() {
	if (playerMeshComponent != nullptr) {
		playerAnimation = playerMeshComponent->GetAnimInstance();
	}
	if (meleeAttackMontage != nullptr && playerAnimation != nullptr) {
		if (!playerAnimation->Montage_IsPlaying(meleeAttackMontage)) {
			playerAnimation->Montage_Play(meleeAttackMontage);
			UE_LOG(LogTemp, Warning, TEXT("Executing melee attack animation correctly"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error trying to load melee anim Montage"));
	}
}

void ANecrognomiconCharacter::ActivatePickupAnimation() {
	if (playerMeshComponent != nullptr) {
		playerAnimation = playerMeshComponent->GetAnimInstance();
	}
	if (pickupItemAnimMontage != nullptr && playerAnimation != nullptr) {
		if (!playerAnimation->Montage_IsPlaying(pickupItemAnimMontage)) {
			playerAnimation->Montage_Play(pickupItemAnimMontage);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error trying to load firing anim Montage"));
	}
}

void ANecrognomiconCharacter::ActivateHurtAnimation() {
	if (playerMeshComponent != nullptr) {
		playerAnimation = playerMeshComponent->GetAnimInstance();
	}
	if (hurtAnimMontage != nullptr && playerAnimation != nullptr) {
		if (!playerAnimation->Montage_IsPlaying(hurtAnimMontage)) {
			playerAnimation->Montage_Play(hurtAnimMontage);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error trying to load firing anim Montage"));
	}
}

void ANecrognomiconCharacter::BindToDelegate() { Die(); }

void ANecrognomiconCharacter::Die() {
	UE_LOG(LogTemp, Warning, TEXT("You have died."));
}

void ANecrognomiconCharacter::ReceiveDamage(float Damage)
{
	ReceiveDamageAmount = Damage;
	// Start timer if none is active
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PainTimer))
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PainTimer, this, &ANecrognomiconCharacter::PainTimer, 1.0f, true);
	}

}

void ANecrognomiconCharacter::PainTimer()
{
	TSet<AActor*> TouchingActors;
	//GetOverlappingActors(TouchingActors, APawn::StaticClass());
	GetOverlappingActors(TouchingActors, ANecrognomiconEnemy::StaticClass());
	float currentEnemy = 0;
	// for each gnome inside de volume we will damage the house
	for (AActor* const A : TouchingActors)
	{
		if (!isPlayerInvulnerable) {
			InflictDamage(A);
		}
	}

	// Stop timer if nothing is overlapping us
	if (TouchingActors.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
	}

}

void ANecrognomiconCharacter::InflictDamage(AActor* Other)
{
	UE_LOG(LogTemp, Error, TEXT("InflictDamage"));
	if (ReceiveDamageAmount > 0.f)
	{
		float DamageAmount = ReceiveDamageAmount * 1.0f;
		// get pawn controller
		AController* DamageInstigator = Other->GetInstigatorController();

		if (DamageInstigator != nullptr)
		{
			if ((Other != nullptr) && (Other != this))
			{
				// Create a damage event  
				TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
				FDamageEvent DamageEvent(ValidDamageTypeClass);

				this->TakeDamage(DamageAmount, DamageEvent, DamageInstigator, Other);		
			}
		}
	}
}

float ANecrognomiconCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// DamageType example and properties
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	DamageTypeCDO->DamageImpulse;
	DamageTypeCDO->bCausedByWorld;

	// Health component
	if (ActualDamage != 0.f && !isPlayerInvulnerable)
	{
		if (hurtAudioComponent != nullptr) {
			if(!hurtAudioComponent->IsPlaying()) {
				hurtAudioComponent = UFMODBlueprintStatics::PlayEventAttached(hurtSound, GetMesh(), "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
			}
		}
		else {
			hurtAudioComponent = UFMODBlueprintStatics::PlayEventAttached(hurtSound, GetMesh(), "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
		}
		if (classesTriggersHurtAnim.Contains(DamageCauser->GetClass())) {
			ActivateHurtAnimation();
		}

		// Set the material to red
		if (dynamicMaterial != nullptr) {
			dynamicMaterial->SetVectorParameterValue("color_fresnel", FLinearColor(0.1, 0.0, 0.0));
			GetWorldTimerManager().SetTimer(TimerHandle_MaterialChangeRed, this, &ANecrognomiconCharacter::ChangeRedMaterial, materialChangeRedDelay, false);
		}

		this->HealthStatusEvent.Broadcast(this->healthComponent->health);
		healthComponent->Damage(ActualDamage);

		if (playerHurt) 
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(playerHurt);
		}

		// if dead destroy
		if (!healthComponent->alive) {	

			//Stop all weapon logic
			StopWeaponLogic();
			
			//Activate the death animation of the player (it is used as a delay too)		
			if (gameMode != nullptr)
			{
				if (!gameMode->GetIsPlayerDead()) {
					timeWaitingForRespawn = 0.f;
					gameMode->SetIsPlayerDead(true);
				}
			}
			
			if (endlessGameMode != nullptr)
			{
				endlessGameMode->CreateLoseWidget();
			}

			ActualDamage = 0.f;
			if (HeartbeatSound != nullptr) {
				// set the parameter that control the beat sound to 1 again so the sound is not activated again when the player is dead
				HeartbeatSound->SetParameter("Health", 1);
			}
		}
		else {
			timeOutOfAction = 0.f;
			UpdateHeartbeatSound();
		}
	}

	return ActualDamage;
}

void ANecrognomiconCharacter::UpdateHeartbeatSound() {
	if (HeartbeatSound != nullptr) {
		// set the parameter that control the beat sound normalizing the health value
		HeartbeatSound->SetParameter("Health", healthComponent->health / healthComponent->defaultHealth);
	}
}

void ANecrognomiconCharacter::ResetCharacterParams() {
	healthComponent->health = healthComponent->defaultHealth;

	UpdateHeartbeatSound();
}


void ANecrognomiconCharacter::CheckIfCanRestoreHealth(float DeltaSeconds) {
	timeOutOfAction += DeltaSeconds;
	timeLasts += DeltaSeconds;
	//Check if the player has exit combat and restores it only if it has passed 1 second
	if (timeOutOfAction >= secondsToExitCombat && timeLasts >= 1.f) {
		healthComponent->health += healthRecoveryPerSecond;
		healthComponent->health = FMath::Min(healthComponent->health, healthComponent->defaultHealth);
		HealthStatusEvent.Broadcast(healthComponent->health);
		UpdateHeartbeatSound();
		timeLasts = 0.f;	
	}
}


USlotManagerComponent* ANecrognomiconCharacter::GetSlotManager() const {
	return slotManager;
}

void ANecrognomiconCharacter::ChangeRedMaterial() {
	GetWorldTimerManager().ClearTimer(TimerHandle_MaterialChangeRed);
	dynamicMaterial->SetVectorParameterValue("color_fresnel", FLinearColor(0.0, 0.0, 0.0));
}

void ANecrognomiconCharacter::StartBlinking() {
	if (dynamicMaterial != nullptr && secondaryDynamicMaterial != nullptr) {
		dynamicMaterial->SetScalarParameterValue("opacity", 0);
		secondaryDynamicMaterial->SetScalarParameterValue("opacity_general", 0);
		GetWorldTimerManager().SetTimer(TimerHandle_Blinking, this, &ANecrognomiconCharacter::ResetBlinking, resetBlinkingDelay, false);
	}
}

void ANecrognomiconCharacter::ResetBlinking() {
	GetWorldTimerManager().ClearTimer(TimerHandle_Blinking);
	if (dynamicMaterial != nullptr && secondaryDynamicMaterial != nullptr) {
		dynamicMaterial->SetScalarParameterValue("opacity", 1);
		secondaryDynamicMaterial->SetScalarParameterValue("opacity_general", 1);
	}
}

void ANecrognomiconCharacter::StopBlinking() {
	if (dynamicMaterial != nullptr && secondaryDynamicMaterial != nullptr) {
		dynamicMaterial->SetScalarParameterValue("opacity", 1);
		secondaryDynamicMaterial->SetScalarParameterValue("opacity_general", 1);
	}
}


void ANecrognomiconCharacter::CheckCameraCollision(float DeltaTime) {
	
	//1º Opcion - Camara en tercera persona
	//2º Opcion - Camara detras del player. Cuando la cámara encuentre algún elemento entre su posición y el player
	//3º Opcion - Camara top-down. En caso que la cámara más cercana no sea suficiente y siga colisionando con objetos

	FHitResult hit;
	FCollisionQueryParams queryParams;
	FCollisionResponseParams responseParams;

	//queryParams.AddIgnoredActors();
	//void AddIgnoredActors(const TArray<AActor*> & InIgnoreActors);
	//void AddIgnoredActors(const TArray<const AActor*> & InIgnoreActors);

	// Current Camera Default values
	FVector CurrentCameraLocation = FollowCamera->RelativeLocation;
	FRotator CurrentCameraRotation = FollowCamera->RelativeRotation;

	// TopDown Camera Default values
	//FVector TopDownCameraPosition = FVector(0.0, 0.0f, 200.0f);
	//FRotator TopDownCameraRotation = FRotator(-70.0, 0.0f, 0.0f);
	//FVector TopDownCameraPosition = FVector(0.0, 0.0f, 75.0f);
	//FVector TopDownCameraPosition = FVector(70.0, 0.0f, 150.0f);
	//FRotator TopDownCameraRotation = FRotator(-55.0, 0.0f, 0.0f);
	
	//UStaticMeshComponent
	

	// Close Camera Default values
	//FVector CloseCameraPosition = FVector(100.0, 0.0f, 50.0f);
	//FVector CloseCameraPosition = FVector(70.0, 0.0f, 150.0f);
	//FVector CloseCameraPosition = FVector(-50.0, 0.0f, 50.0f);
	FVector CloseCameraPosition = FVector(-50.0, 0.0f, 40.0f);

	float approachThreshold = 1.0f;
	float lerpAlpha = FMath::Clamp(DeltaTime * 0.8f, 0.0f, 1.0f);
	
	bool hitted = GetWorld()->LineTraceSingleByChannel(hit,
		FollowCamera->GetComponentLocation()/* - FollowCamera->GetForwardVector() * 100 */,
		FollowCamera->GetForwardVector() + GetActorLocation(),
		ECollisionChannel::ECC_Pawn,
		queryParams,
		responseParams);

	AActor* actor;
	UClass* clas;
	UStaticMesh* usmesh;
	AStaticMeshActor* mesh = Cast<AStaticMeshActor>(hit.GetActor());
	if (hitted)
	{
		actor = hit.GetActor();
		clas = hit.GetActor()->GetClass();
		if (mesh != nullptr) {
			usmesh = mesh->GetStaticMeshComponent()->GetStaticMesh();
		}
	}


	bool isNotSafeCurrentLocation = hitted && hit.GetActor() != this
		// Ignoramos Clases de los BP
		&& !classesIgnoredByCamera.Contains(hit.GetActor()->GetClass())
		// Ignoramos StaticMeshes
		&& ((mesh != nullptr && !staticMeshesIgnoredByCamera.Contains(mesh->GetStaticMeshComponent()->GetStaticMesh()))
		 || mesh == nullptr)
		;

	if (ghostCameraCreated) {
		// 1.- Intentar alejar del player
		FollowCamera->SetRelativeLocation(FMath::Lerp(CurrentCameraLocation, CameraLocation, lerpAlpha));
		FollowCamera->SetRelativeRotation(FMath::Lerp(FQuat(CurrentCameraRotation), FQuat(CameraRotation), lerpAlpha));

		hitted = GetWorld()->LineTraceSingleByChannel(hit,
			FollowCamera->GetComponentLocation(),
			FollowCamera->GetForwardVector() + GetActorLocation(),
			ECollisionChannel::ECC_Pawn,
			queryParams,
			responseParams);		

		AStaticMeshActor* mesh = Cast<AStaticMeshActor>(hit.GetActor());
		bool isNotSafeBackLocation = hitted && hit.GetActor() != this
			// Ignoramos Clases de los BP
			&& !classesIgnoredByCamera.Contains(hit.GetActor()->GetClass())
			// Ignoramos StaticMeshes
			&& ((mesh != nullptr && !staticMeshesIgnoredByCamera.Contains(Cast<AStaticMeshActor>(hit.GetActor())->GetStaticMeshComponent()->GetStaticMesh()))
				|| mesh == nullptr);

		if (isNotSafeBackLocation) {
			//UE_LOG(LogTemp, Error, TEXT("CAMARA INTERSECTANDO CON : %s"), *hit.GetActor()->GetFName().ToString());

			// Si no se puede volver entonces recuperamos la posicion/rotacion actual
			FollowCamera->SetRelativeLocation(CurrentCameraLocation);
			FollowCamera->SetRelativeRotation(CurrentCameraRotation);
		} else {

			/////////////////////////// LOG DE COMPROBACION ////////////////////////////////////
			//if (hitted && hit.GetActor() != this && classesIgnoredByCamera.Contains(hit.GetActor()->GetClass())) {
			//	UE_LOG(LogTemp, Error, TEXT("CAMARA IGNORANDO CLASE: %s"), *hit.GetActor()->GetFName().ToString());
			//}
			//else if (hitted && hit.GetActor() != this && staticMeshesIgnoredByCamera.Contains(Cast<AStaticMeshActor>(hit.GetActor())->GetStaticMeshComponent()->GetStaticMesh())) {
			//	UE_LOG(LogTemp, Error, TEXT("CAMARA IGNORANDO MESH: %s"), *hit.GetActor()->GetFName().ToString());
			//}
			////////////////////////////////////////////////////////////////////////////////////

			// Si podemos volver cancelamos el flag para mantener el acercamiento
			// y no aplicar posteriormente el alejamiento
			isNotSafeCurrentLocation = false;

			float distance = (CameraLocation - FollowCamera->RelativeLocation).SizeSquared();
			// Permitimos mover la cámara de nuevo
			//if (distance <= approachThreshold * 2 && !CameraBoom->bUsePawnControlRotation) {
			//}

			// Si se ha vuelto cerca de la posicion así que se vuelve a la cámara por defecto
			if (distance <= approachThreshold) {
				//DEFAULT THIRD PERSON CAMERA MODE
				FollowCamera->SetRelativeLocation(CameraLocation);
				FollowCamera->SetRelativeRotation(CameraRotation);
				ghostCameraCreated = false;
				//CameraBoom->bUsePawnControlRotation = true;
				//Cast<ANecrognomiconController>(GetController())->canLookUp = true;
			}
		}
	}

	if (!ghostCameraCreated) {
		// 2.- Intentar quedarse como está
		if (isNotSafeCurrentLocation) {
			ghostCameraCreated = true;
			//CameraBoom->bUsePawnControlRotation = false;
			//Cast<ANecrognomiconController>(GetController())->canLookUp = false;
		}
	}

	if (ghostCameraCreated) {
		// 3.- Acercar al player
		if (isNotSafeCurrentLocation){
			FollowCamera->SetRelativeLocation(FMath::Lerp(CurrentCameraLocation, CloseCameraPosition, lerpAlpha));

			FRotator rot = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation() + CameraBoom->RelativeLocation + FollowCamera->RelativeLocation,
				GetActorLocation() + GetActorLocation().ForwardVector * 150 + GetActorLocation().UpVector * 70
			);
			FollowCamera->SetRelativeRotation(FMath::Lerp(FQuat(CurrentCameraRotation), FQuat(rot), 0.9 * DeltaTime));

			//FollowCamera->SetRelativeLocation(FMath::Lerp(CurrentCameraLocation, TopDownCameraPosition, lerpAlpha));
			//FollowCamera->SetRelativeRotation(FMath::Lerp(FQuat(CurrentCameraRotation), FQuat(TopDownCameraRotation), lerpAlpha));

			//float distance = (CloseCameraPosition - FollowCamera->RelativeLocation).SizeSquared();
			//if (distance <= approachThreshold * 4) {
			//	FollowCamera->SetRelativeRotation(FMath::Lerp(FQuat(CurrentCameraRotation), FQuat(TopDownCameraRotation), lerpAlpha));
			//	FollowCamera->SetRelativeRotation(
			//		UKismetMathLibrary::FindLookAtRotation(
			//			GetActorLocation() + CameraBoom->RelativeLocation + FollowCamera->RelativeLocation,
			//			GetActorLocation() + GetActorLocation().ForwardVector * 75 + GetActorLocation().UpVector * 75
			//		));
			//}
		}
	}	
}


void ANecrognomiconCharacter::PlayErrorSound() {
	if (errorAudioComponent != nullptr) {
		if (!errorAudioComponent->IsPlaying()) {
			errorAudioComponent->Play();
		}
	}
}

void ANecrognomiconCharacter::StopOngoingActions() {
	SecondaryActionStop();
	ANecrognomiconController* NecroController = Cast<ANecrognomiconController>(GetController());
	NecroController->CallStopRunning();
}

/*
void ANecrognomiconCharacter::CheckCameraCollision(float DeltaTime) {

	//1º Opcion - Camara en tercera persona
	//2º Opcion - Camara detras del player. Cuando la cámara encuentre algún elemento entre su posición y el player
	//3º Opcion - Camara top-down. En caso que la cámara más cercana no sea suficiente y siga colisionando con objetos

	FHitResult hit;
	FCollisionQueryParams queryParams;
	FCollisionResponseParams responseParams;

	// Current Camera Default values
	FVector CurrentCameraLocation = FollowCamera->RelativeLocation;
	FRotator CurrentCameraRotation = FollowCamera->RelativeRotation;

	// Close Camera Default values
	FVector CloseCameraPosition = FVector(0.0, 0.0f, 50.0f);

	// TopDown Camera Default values
	FVector TopDownCameraPosition = FVector(0.0, 0.0f, 400.0f);
	FQuat TopDownCameraRotation = FQuat(FRotator(-70.0, 0.0f, 0.0f));

	if (ghostCameraCreated) {
		//THIRD PERSON CAMERA MODE
		FollowCamera->SetRelativeRotation(CameraRotation);
		FollowCamera->SetRelativeLocation(CameraLocation);
		ghostCameraCreated = false;
		CameraBoom->bUsePawnControlRotation = true;
	}

	bool hitted = false;

	if (!ghostCameraCreated) {

		// Lanzar rayo entre camara principal y player
		hitted = GetWorld()->LineTraceSingleByChannel(
			hit,
			FollowCamera->GetComponentLocation(),
			FollowCamera->GetForwardVector() + GetActorLocation(),
			ECollisionChannel::ECC_Pawn,
			queryParams,
			responseParams);
		if (hitted && hit.GetActor() != this && !classesIgnoredByCamera.Contains(hit.GetActor()->GetClass())) {
			//CLOSE CAMERA MODE
			//FollowCamera->SetRelativeLocation(FMath::Lerp(CurrentCameraLocation, CloseCameraPosition, 0.15f));
			FollowCamera->SetRelativeLocation(FMath::Lerp(CurrentCameraLocation, TopDownCameraPosition, 0.05f));
			FollowCamera->SetRelativeRotation(FMath::Lerp(FQuat(CurrentCameraRotation), TopDownCameraRotation, 0.05f));
			//FollowCamera->SetRelativeLocation(CloseCameraPosition);
			ghostCameraCreated = true;
			CameraBoom->bUsePawnControlRotation = false;
		}

		// Lanzar rayo entre camara principal y player
		hitted = GetWorld()->LineTraceSingleByChannel(
			hit,
			FollowCamera->GetComponentLocation(),
			FollowCamera->GetForwardVector() + GetActorLocation(),
			ECollisionChannel::ECC_Pawn,
			queryParams,
			responseParams);

		// Si choca con algo que no sea el actor o elementos a ignorar
		if (hitted && hit.GetActor() != this && !classesIgnoredByCamera.Contains(hit.GetActor()->GetClass())) {
			//TOP DOWN CAMERA MODE
			FollowCamera->SetRelativeRotation(TopDownCameraRotation);
			FollowCamera->SetRelativeLocation(TopDownCameraPosition);
			ghostCameraCreated = true;
			CameraBoom->bUsePawnControlRotation = false;
		}
	}
}*/


//void ANecrognomiconCharacter::PostInitializeComponents() {
//
//	if (mesh != nullptr) {
//		UMaterialInterface* material = mesh->GetMaterial(0);
//		if (material) {
//			dynamicMaterial = UMaterialInstanceDynamic::Create(material, staticMeshComponent);
//
//			if (dynamicMaterial != nullptr) {
//				mesh->SetMaterial(0, dynamicMaterial);
//			}
//		}
//	}
//	Super::PostInitializeComponents();
//}