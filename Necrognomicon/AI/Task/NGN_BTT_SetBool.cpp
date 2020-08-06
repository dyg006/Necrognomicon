// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_SetBool.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UNGN_BTT_SetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolKey.SelectedKeyName, boolValue);

	return EBTNodeResult::Succeeded;
}