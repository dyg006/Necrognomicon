// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTS_PerceivePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/NGN_GnomeController.h"

UNGN_BTS_PerceivePlayer::UNGN_BTS_PerceivePlayer()
{
	
}

void UNGN_BTS_PerceivePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementaci�n de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del GnomeController
	ANGN_GnomeController* gnomeController = Cast<ANGN_GnomeController>(OwnerComp.GetOwner());

	//Llamamos al m�todo CheckPlayerPerceived del GnomeController que tiene toda la l�gica para determinar si el
	// jugador est� cerca o no y configurar el Key correspontiente del Blackboard
	gnomeController->CheckPlayerPerceived();
}


