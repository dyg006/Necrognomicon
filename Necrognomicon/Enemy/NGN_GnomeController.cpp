// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_GnomeController.h"
#include "Components/PlayerAttackingSlot.h"
#include "Components/SlotManagerComponent.h"
#include "Traps/BPADiscoMushroomTrap.h"
#include "AI/PathFollowing/PathFollowing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Buildings/House.h"
#include "GameMode/NecrognomiconGameMode.h"
#include "TimerManager.h"
#include "GameMode/NGN_GameInstance.h"

void ANGN_GnomeController::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (_behaviorTree == nullptr)
	{
		return;
	}
		
	RunBehaviorTree(_behaviorTree);
	*/
	_blackboard = GetBlackboardComponent();
	/*
	if (blackboard == nullptr)
	{
		return;
	}
	blackboard->SetValueAsBool("IsDeactivated", false);
	blackboard->SetValueAsBool("CanAttackPlayer", true);
	blackboard->SetValueAsBool("IsLost", false);


	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Player", players);
	blackboard->SetValueAsObject("EnemyActor", players[0]);
	*/

	_navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	_gnomeManager = Cast<UNGN_GameInstance>(GetGameInstance())->GetGnomeManager();

	m_previousLocation = FVector::ZeroVector;
}


void ANGN_GnomeController::SetHouse(AHouse* house)
{
	_blackboard->SetValueAsObject("TargetHouse", house);
}

void ANGN_GnomeController::SetPath(ANGN_Path * path)
{
	TArray<UPathFollowing*> pathFollowingArray;
	GetPawn()->GetComponents(pathFollowingArray);

	if (pathFollowingArray.Num() == 0)
	{
		return;
	}

	pathFollowingArray[0]->SetPath(path);
	if (path == nullptr)
	{
		_blackboard->ClearValue("Path");
		_blackboard->SetValueAsBool("PathFinished", false);
	}
	else
	{
		_blackboard->SetValueAsObject("Path", path);
	}
}

void ANGN_GnomeController::SetEndless(bool bEndless)
{
	_endless = bEndless;
}


void ANGN_GnomeController::RemovePath()
{
	TArray<UPathFollowing*> pathFollowingArray;
	GetPawn()->GetComponents(pathFollowingArray);

	if (pathFollowingArray.Num() > 0)
	{
		pathFollowingArray[0]->SetPath(nullptr);
		_blackboard->ClearValue("Path");
	}
}

void ANGN_GnomeController::SetCanAttackPlayer(bool canAttack)
{
	_blackboard->SetValueAsBool("CanAttackPlayer", canAttack);
}

bool ANGN_GnomeController::GetCanAttackPlayer()
{
	return _blackboard->GetValueAsBool("CanAttackPlayer");
}

void ANGN_GnomeController::SetIsAstrayed(bool isAstrayed)
{
	_blackboard->SetValueAsBool("IsAstrayed", isAstrayed);
}


bool ANGN_GnomeController::GetIsAstrayed()
{
	return _blackboard->GetValueAsBool("IsAstrayed");
}


void ANGN_GnomeController::SetIsDancing(bool isDancing)
{
	_blackboard->SetValueAsBool("IsDancing", isDancing);
}


bool ANGN_GnomeController::GetIsDancing()
{
	return _blackboard->GetValueAsBool("IsDancing");
}

void ANGN_GnomeController::SetAIActive(bool active)
{
	_blackboard->SetValueAsBool("IsDeactivated", !active);
}

void ANGN_GnomeController::SetDancingTime(float dancingTime)
{
	_blackboard->SetValueAsFloat("DancingTime", dancingTime);
}


void ANGN_GnomeController::CheckPlayerPerceived()
{
	ANecrognomiconGameMode* gm = Cast<ANecrognomiconGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		if (GetIsAstrayed() || gm->GetIsPlayerDead())
		{
			_blackboard->SetValueAsBool("IsPlayerPerceived", false);
			_blackboard->SetValueAsBool("IsPlayerClose", false);
			return;
		}
	}

	AActor* player = Cast<AActor>(_blackboard->GetValueAsObject("EnemyActor"));

	FVector direction = player->GetActorLocation() - GetPawn()->GetActorLocation();

	float distance = direction.Size();

	bool canAttackByDistance;

	if (distance > farRange)
	{
		canAttackByDistance = false;
		_blackboard->SetValueAsBool("IsPlayerClose", false);
	}
	else if (distance < closeRange)
	{
		canAttackByDistance = true;
		_blackboard->SetValueAsBool("IsPlayerClose", true);
	}
	else
	{
		FVector lookingAt = GetPawn()->GetViewRotation().Vector();
		
		// angle = acos( (a · b) / (|a| * |b|))
		float angle = FMath::Acos(
			FVector::DotProduct(direction, lookingAt)
			/ (direction.Size() * lookingAt.Size()));

		angle = (angle / PI) * 180.f;

		canAttackByDistance = angle < fovAngle / 2.f;
		_blackboard->SetValueAsBool("IsPlayerClose", true);
	}

	_blackboard->SetValueAsBool("IsPlayerPerceived", canAttackByDistance);

}

void ANGN_GnomeController::CheckStucked(float StuckDistance)
{
	FVector ownLocation = GetPawn()->GetActorLocation();

	if (!m_previousLocation.Equals(FVector::ZeroVector))
	{
		// check if we are in the same location
		if (FVector::Distance(ownLocation, m_previousLocation) < StuckDistance)
		{
			_blackboard->SetValueAsBool("IsStucked", true);
		}
		else
		{
			_blackboard->SetValueAsBool("IsStucked", false);
		}
	}

	m_previousLocation = ownLocation;
}

void ANGN_GnomeController::AskCanAttackPlayer()
{
	ANecrognomiconEnemy* enemy = Cast<ANecrognomiconEnemy>(GetPawn());
	
	if (enemy->HealthComponent->alive) {
		bool canAttackPlayer = _gnomeManager->TryAttackPlayer(GetPawn()->GetClass());

		SetCanAttackPlayer(canAttackPlayer);
	}
	else {
		SetCanAttackPlayer(false);
	}
	
}

void ANGN_GnomeController::UnregisterFromPlayerSlot()
{
	if (GetCanAttackPlayer())
	{

			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Me dessuscribo del slot")));
		
		_gnomeManager->UnregisterGnome(GetPawn()->GetClass(), Cast<APlayerAttackingSlot>(_blackboard->GetValueAsObject("PlayerSlot")));
		SetCanAttackPlayer(false);
	}
}


/// <summary>
/// Checks the astrayed.
/// </summary>
/// <param name="followPositionKeyName">Name of the follow position key.</param>
void ANGN_GnomeController::CheckAstrayed(FName followPositionKeyName)
{
	FVector targetPosition = _blackboard->GetValueAsVector(followPositionKeyName);
	FVector ownPosition = GetPawn()->GetActorLocation();
	float distance = (targetPosition - ownPosition).Size();

	if (distance >= maxAstray) {
		/*UE_LOG(LogTemp, Warning, TEXT("Setting Astrayed Distance: %.2f"), distance);

		UE_LOG(LogTemp, Warning, TEXT("Current Position: (%.2f, %.2f, %.2f) Next Position: (%.2f, %.2f, %.2f)"),
			ownPosition.X, ownPosition.Y, ownPosition.Z,
			targetPosition.X, targetPosition.Y, targetPosition.Z);
			*/
		SetIsAstrayed(true);
		//SetCanAttackPlayer(false);

		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ANGN_GnomeController::SetIsAstrayed, false);
		GetWorldTimerManager().SetTimer(_timerHandle, timerDelegate, ignoreTime, false, -1.0f);

		_blackboard->SetValueAsBool("IsRecentlyAstrayed", true);
	}

}

void ANGN_GnomeController::UpdatePathFollowPosition(FName followPositionKeyName, FName resetPositionKeyName)
{

	TArray<UPathFollowing*> pathFollowingArray;
	APawn* pawn = GetPawn();
	pawn->GetComponents(pathFollowingArray);

	if (pathFollowingArray.Num() == 0)
	{
		return;
	}

	bool resetPosition = _blackboard->GetValueAsBool(resetPositionKeyName);

	FVector targetPosition;

	if (!pathFollowingArray[0]->GetSteering(targetPosition, resetPosition))
	{
		return;
	}

	if (resetPosition)
	{
		_blackboard->SetValueAsBool(resetPositionKeyName, false);
	}

	FNavLocation result;
	_navSys->GetRandomReachablePointInRadius(targetPosition, 100, result);

	targetPosition = result.Location;

	_blackboard->SetValueAsVector(followPositionKeyName, targetPosition);
#if ENABLE_DRAW_DEBUG && WITH_EDITOR
	DrawDebugSphere(GetWorld(), targetPosition, 10.f, 10, FColor::Yellow);
#endif

	if (pathFollowingArray[0]->IsFinished())
	{
		//SetPath(nullptr);
		_blackboard->SetValueAsBool("PathFinished", true);
	}
}

void ANGN_GnomeController::MushroomInRadius(AActor* discoMushroom, float time, float radius)
{

	if (_blackboard == nullptr) {
		return;
	}
	if (!GetIsDancing()) {
		SetDancingTime(time);
		//UE_LOG(LogTemp, Warning, TEXT("Gnome entering on the discoMushroom"), time);
		currentMushroomObjective = Cast<ABPADiscoMushroomTrap>(discoMushroom);
		if (currentMushroomObjective != nullptr) {
			currentMushroomObjective->EnemyStartedDancing();
			_mushroomRadius = radius;
		}
		SetIsDancing(true);
	}
}

void ANGN_GnomeController::SearchDancingSpot()
{
	FVector offset = FVector(FMath::RandPointInCircle(_mushroomRadius), 0.f);
	FVector location = currentMushroomObjective->GetActorLocation() + offset;

	_blackboard->SetValueAsVector("DancingSpot", location);
}

void ANGN_GnomeController::SearchHouseSpot()
{
	AHouse* targetHouse = Cast<AHouse>(_blackboard->GetValueAsObject("TargetHouse"));

	if (targetHouse != nullptr) {
		_blackboard->SetValueAsVector("HouseSlot", targetHouse->GetNextSlot());
	}
	
}

void ANGN_GnomeController::SearchPlayerSpot()
{
	ANecrognomiconCharacter* targetPlayer = Cast<ANecrognomiconCharacter>(_blackboard->GetValueAsObject("EnemyActor"));

	if (targetPlayer != nullptr) {
		APlayerAttackingSlot* slot = targetPlayer->GetSlotManager()->GetClosestSlot(GetPawn()->GetActorLocation());
		if (slot != nullptr) {
			_blackboard->SetValueAsObject("PlayerSlot", slot);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("el slot es NULL"));
		}
	}

}

void ANGN_GnomeController::StopDancing()
{

	if (currentMushroomObjective != nullptr) {
		currentMushroomObjective->EnemyFinishedDancing();
	}
	SetIsDancing(false);
}

FVector ANGN_GnomeController::GetRandomPointNavigable(FVector origin, float radius)
{
	FNavLocation result;
	_navSys->GetRandomPointInNavigableRadius(origin, radius, result);
	return result.Location;
}

void ANGN_GnomeController::SetCustomDepth(bool newValue)
{
	ANecrognomiconEnemy* enemy = Cast<ANecrognomiconEnemy>(GetPawn());
	// needed for oclussion
	//enemy->GetMesh()->SetRenderCustomDepth(newValue);
	if (newValue) {
		enemy->GetMesh()->SetCustomDepthStencilValue(20);
	}
	else
	{
		enemy->GetMesh()->SetCustomDepthStencilValue(0);
	}
}

void ANGN_GnomeController::JumpForward(float distance, float duration)
{
	ANecrognomiconEnemy* enemy = Cast<ANecrognomiconEnemy>(GetPawn());
	enemy->JumpForward(distance, duration);
}

FVector ANGN_GnomeController::GetBackLocation(float Radius)
{
	return GetPawn()->GetActorLocation() + ( GetPawn()->GetActorForwardVector() * -1.f * Radius );
}
