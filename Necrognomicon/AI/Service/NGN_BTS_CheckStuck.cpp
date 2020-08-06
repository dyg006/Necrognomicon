// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTS_CheckStuck.h"

#include "Enemy/NGN_GnomeController.h"

UNGN_BTS_CheckStuck::UNGN_BTS_CheckStuck()
{

}

void UNGN_BTS_CheckStuck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementación de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del GnomeController
	ANGN_GnomeController* gnomeController = Cast<ANGN_GnomeController>(OwnerComp.GetOwner());

	gnomeController->CheckStucked(StuckDistance);
}
