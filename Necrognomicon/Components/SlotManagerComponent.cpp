// Fill out your copyright notice in the Description page of Project Settings.

#include "SlotManagerComponent.h"
#include "Math/NumericLimits.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "GameMode/NGN_GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Character/NecrognomiconCharacter.h"
#include "PlayerAttackingSlot.h"
#include "GameMode/NGN_GameInstance.h"

// Sets default values for this component's properties
USlotManagerComponent::USlotManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USlotManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	gnomeManager = Cast<UNGN_GameInstance>(GetWorld()->GetGameInstance())->GetGnomeManager();

	necroCharacter = Cast<ANecrognomiconCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

//PostInitializeComponents

// Called every frame
void USlotManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	/*if (gnomeManager) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Numero de slots actuales %d"), gnomeManager->GetTotalCurrentGnomes()));
	}*/
	SetWorldRotation(FRotator::ZeroRotator);
	//if (necroCharacter != nullptr) {

	//	//Debug render
	//	for (APlayerAttackingSlot* slot : playerSlots) {
	//		DrawDebugLine(GetWorld(), GetComponentLocation(), slot->GetActorLocation(), FColor::Orange);
	//	}
	//}
}

void USlotManagerComponent::CreatePlayerSlots() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Slots");
	
	if (gnomeManager == nullptr) {
		gnomeManager = Cast<UNGN_GameInstance>(GetWorld()->GetGameInstance())->GetGnomeManager();
	}

	if (gnomeManager != nullptr) {
		numberOfSlots = gnomeManager->GetTotalMaxGnomes();

		if (numberOfSlots > 0) {
			for (int i = 0; i < numberOfSlots; ++i) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Creating 1 Slot.");
				playerSlots.Add(NewObject<APlayerAttackingSlot>(this, APlayerAttackingSlot::StaticClass()));
				//playerSlots.Add(GetWorld()->SpawnActor<APlayerAttackingSlot>(APlayerAttackingSlot::StaticClass(), GetComponentLocation(),GetComponentRotation()));
				//playerSlots[i]->SetupAttachment(this);
				//playerSlots[i]->RootComponent->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
				playerSlots[i]->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
				playerSlots[i]->RegisterAllComponents();			
				float angleAroundCircle = (float)i / (float)numberOfSlots * PI * 2;
				float radius = characterRadius / FMath::Sin(PI / (float)numberOfSlots);

				radius = FMath::Min(radius, _maxRadius);

				float x = radius * FMath::Cos(angleAroundCircle);
				float y = radius * FMath::Sin(angleAroundCircle);
				//playerSlots[i]->SetRelativeLocation(FVector(x, y, 0.f)); //360 / (i + 1)
				//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("Posicion del slot: x: %lf , y: %lf"), x, y));
				playerSlots[i]->SetActorRelativeLocation(FVector(x, y, -100.f)); //360 / (i + 1)
				//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("Posicion del slot: x: %lf , y: %lf, z:%lf"), playerSlots[i]->GetActorLocation().X, playerSlots[i]->GetActorLocation().Y, playerSlots[i]->GetActorLocation().Z));

			}	
		}
	}	
	//DrawCircle(pd, FVector(0, 360 / numberOfSlots, 0));
}

//void USlotManagerComponent::CreateFixedPlayerSlots() {
//
//	if (gnomeManager == nullptr) {
//		gnomeManager = Cast<UNGN_GameInstance>(GetWorld()->GetGameInstance())->GetGnomeManager();
//	}
//
//	if (gnomeManager != nullptr ) {
//		numberOfSlots = endlessModePlayerSlots;
//
//		if (numberOfSlots > 0) {
//			for (int i = 0; i < numberOfSlots; ++i) {
//				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Creating 1 Slot.");
//				playerSlots.Add(NewObject<UPlayerAttackingSlot>(this, UPlayerAttackingSlot::StaticClass()));
//				playerSlots[i]->SetupAttachment(this);
//				playerSlots[i]->RegisterComponent();
//				float angleAroundCircle = (float)i / (float)numberOfSlots * PI * 2;
//				float radius = characterRadius / FMath::Sin(PI / (float)numberOfSlots);
//
//				float x = radius * FMath::Cos(angleAroundCircle);
//				float y = radius * FMath::Sin(angleAroundCircle);
//				playerSlots[i]->SetRelativeLocation(FVector(x, y, 0.f));
//
//			}
//		}
//	}
//}

void USlotManagerComponent::RemovePlayerSlots() {
	for (APlayerAttackingSlot* slot : playerSlots) {
		slot->Destroy();
	}
	playerSlots.Empty();
}

void USlotManagerComponent::SetFreeAllSlots() {
	for (APlayerAttackingSlot* slot : playerSlots) {
		slot->isFree = true;
	}
}

APlayerAttackingSlot* USlotManagerComponent::GetClosestSlot(const FVector& enemyPosition)
{
	float minDistance = FLT_MAX;
	float currentDistance;
	int closestIndex = -1;

	for (int i = 0; i < numberOfSlots; ++i) {
		if (playerSlots[i]->isFree) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "FREE SLOT");
			currentDistance = FVector::DistSquared(enemyPosition, playerSlots[i]->GetActorLocation());
			if (currentDistance <= minDistance) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Actualizamos la distancia minima");
				minDistance = currentDistance;
				closestIndex = i;
			}
		}
	}
	// if there is a free slot we get the closest one and the state to busy
	if (closestIndex >= 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "LE DEVUELVO EL SLOT AL GNOMITO");
		playerSlots[closestIndex]->isFree = false;
		return playerSlots[closestIndex];
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "TOMA TE DOY UN NULLPTR $@%$");
		return nullptr;
	}
}