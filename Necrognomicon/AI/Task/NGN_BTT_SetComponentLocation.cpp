// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_SetComponentLocation.h"
//#include "Components/SceneComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

//UNGN_BTT_SetComponentLocation::UNGN_BTT_SetComponentLocation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
//{
//	NodeName = "NGN_SetComponentLocation";
//	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UNGN_BTT_SetComponentLocation, BlackboardKey), USceneComponent::StaticClass());
//
//
//}

EBTNodeResult::Type UNGN_BTT_SetComponentLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//OwnerComp.GetBlackboardComponent()->SetValueAsObject(componentKey.SelectedKeyName, component);

	sceneComponent = Cast<USceneComponent>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(componentKey.SelectedKeyName));

	if (sceneComponent != nullptr) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(vectorKey.SelectedKeyName, sceneComponent->GetComponentLocation());

		return EBTNodeResult::Succeeded;
	}
	else {
		return EBTNodeResult::Failed;
	}


	
}