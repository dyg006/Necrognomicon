
#include "NGN_BTT_RotateToFaceBBEntry.h"
#include "Components/SceneComponent.h"

UNGN_BTT_RotateToFaceBBEntry::UNGN_BTT_RotateToFaceBBEntry(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "NGN_RotateToFaceBBEntry";
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UNGN_BTT_RotateToFaceBBEntry, BlackboardKey), USceneComponent::StaticClass());
}
