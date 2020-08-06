// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_SearchHouseSpot.h"

#include "Enemy/NGN_GnomeController.h"

EBTNodeResult::Type UNGN_BTT_SearchHouseSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner())->SearchHouseSpot();

	return EBTNodeResult::Succeeded;
}
