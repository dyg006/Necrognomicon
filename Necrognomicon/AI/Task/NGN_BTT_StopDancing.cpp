#include "NGN_BTT_StopDancing.h"
#include "Enemy/NGN_GnomeController.h"
#include "AIController.h"

EBTNodeResult::Type UNGN_BTT_StopDancing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANGN_GnomeController* gnomeController = Cast<ANGN_GnomeController>(OwnerComp.GetOwner());

	if (gnomeController != nullptr) {
		gnomeController->StopDancing();
		Cast<ANecrognomiconEnemy>(OwnerComp.GetAIOwner()->GetPawn())->StopDancing();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}