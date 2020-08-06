// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_GnomeController_Endless.h"

#include "BehaviorTree/BlackboardComponent.h"
//#include "Perception/AIPerceptionComponent.h"
#include "GameMode/NGN_GameInstance.h"

void ANGN_GnomeController_Endless::BeginPlay()
{
	Super::BeginPlay();

	this->RunBehaviorTree(BehaviorTree);

	Blackboard = GetBlackboardComponent();
	Blackboard->SetValueAsObject("EnemyActor", GetWorld()->GetFirstPlayerController()->GetPawn());
	
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	//AIPerception = GetPerceptionComponent();
}

FVector ANGN_GnomeController_Endless::GetRandomPointNavigable(FVector origin, float radius)
{
	FNavLocation result;
	NavSys->GetRandomPointInNavigableRadius(origin, radius, result);
	return result.Location;
}
