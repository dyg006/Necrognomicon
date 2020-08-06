// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTS_UpdateVectorLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UNGN_BTS_UpdateVectorLocation::UNGN_BTS_UpdateVectorLocation()
{
	bCreateNodeInstance = true;;
}


void UNGN_BTS_UpdateVectorLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementación de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	sceneComponent = Cast<USceneComponent>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(componentKey.SelectedKeyName));

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(vectorKey.SelectedKeyName, sceneComponent->GetComponentLocation());

}