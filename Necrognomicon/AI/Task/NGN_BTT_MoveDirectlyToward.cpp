// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_BTT_MoveDirectlyToward.h"
#include "Components/SceneComponent.h"

UNGN_BTT_MoveDirectlyToward::UNGN_BTT_MoveDirectlyToward(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "NGN_MoveDirectlyToward";
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UNGN_BTT_MoveDirectlyToward, BlackboardKey), USceneComponent::StaticClass());
}
