// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NecrognomiconEnemy.h"
#include "AIController.h"

EBTNodeResult::Type UNGN_BTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* objective = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(attackObjective.SelectedKeyName));


	Cast<ANecrognomiconEnemy>(OwnerComp.GetAIOwner()->GetPawn())->Attack(objective);

	return EBTNodeResult::Succeeded;
}
