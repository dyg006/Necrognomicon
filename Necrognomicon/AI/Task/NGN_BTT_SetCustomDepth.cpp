// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_SetCustomDepth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_SetCustomDepth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner())->SetCustomDepth(NewCustomDepth);

	return EBTNodeResult::Succeeded;
}
