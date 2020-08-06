// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_BackLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_BackLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANGN_GnomeController* controller = Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner());
	
	FVector targetLocation = controller->GetBackLocation(Radius);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultLocation.SelectedKeyName, targetLocation);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ForwardLocation.SelectedKeyName, controller->GetPawn()->GetActorLocation());

	return EBTNodeResult::Succeeded;
}