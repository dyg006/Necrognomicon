// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NGN_BTT_ApplySteering.generated.h"

/**
 * 
 */
UCLASS()
class NECROGNOMICON_API UNGN_BTT_ApplySteering : public UBTTaskNode
{
	GENERATED_BODY()

public:

	/// <summary>
	/// Gets the static description.
	/// </summary>
	/// <returns></returns>
	//FString GetStaticDescription() const override;

	/// <summary>
	/// Executes the task.
	/// </summary>
	/// <param name="OwnerComp">The owner comp.</param>
	/// <param name="NodeMemory">The node memory.</param>
	/// <returns>Returns Succeded, Failed or In Progress</returns>
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
