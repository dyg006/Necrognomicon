// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANGN_GnomeController* controller = Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner());
	FVector origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(OriginLocation.SelectedKeyName);
	float Radius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(WanderRadius.SelectedKeyName);
	bool IsStatic = OwnerComp.GetBlackboardComponent()->GetValueAsBool(Static.SelectedKeyName);

	if (!IsStatic)
	{
		origin = controller->GetPawn()->GetActorLocation();
	}
	
	FVector targetLocation = controller->GetRandomPointNavigable(origin, Radius);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultLocation.SelectedKeyName, targetLocation);

	return EBTNodeResult::Succeeded;
}