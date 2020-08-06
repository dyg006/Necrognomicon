// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_Dance.h"
#include "Enemy/NecrognomiconEnemy.h"
#include "AIController.h"

EBTNodeResult::Type UNGN_BTT_Dance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<ANecrognomiconEnemy>(OwnerComp.GetAIOwner()->GetPawn())->StartDancing();

	return EBTNodeResult::Succeeded;
}
