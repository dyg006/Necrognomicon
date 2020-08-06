// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_UnregisterFromPlayerSlot.h"

#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_UnregisterFromPlayerSlot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner())->UnregisterFromPlayerSlot();

	return EBTNodeResult::Succeeded;
}
