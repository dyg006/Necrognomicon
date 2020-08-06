// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_SearchDancingSpot.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_SearchDancingSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner())->SearchDancingSpot();

	return EBTNodeResult::Succeeded;
}
