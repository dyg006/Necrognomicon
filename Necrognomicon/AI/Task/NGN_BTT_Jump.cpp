// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_Jump.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANGN_GnomeController* controller = Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner());

	controller->JumpForward(Distance, Duration);

	return EBTNodeResult::Succeeded;
}
