// Fill out your copyright notice in the Description page of Project Settings.
#include "NGN_BTS_CheckAstrayed.h"
#include "AI/PathFollowing/PathFollowing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UNGN_BTS_CheckAstrayed::UNGN_BTS_CheckAstrayed()
{
	bCreateNodeInstance = true;;
}

void UNGN_BTS_CheckAstrayed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementación de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del GnomeController
	ANGN_GnomeController* gnomeController = Cast<ANGN_GnomeController>(OwnerComp.GetOwner());

	//Llamamos al método CheckAstrayed del GnomeController que tiene toda la lógica para determinar si
	//nos hemos alejado demasiado del path y configurar el Key correspontiente del Blackboard
	gnomeController->CheckAstrayed(followPosition.SelectedKeyName);


}
