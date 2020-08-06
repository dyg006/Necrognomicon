// Fill out your copyright notice in the Description page of Project Settings.

//#include "BehaviorTree/BehaviorTree.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BTTask_SteeringBehaviour.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Enemy/NGN_GnomeController.h"
#include "GameFramework/Actor.h"
#include "SteeringManager.h"


EBTNodeResult::Type UBTTask_SteeringBehaviour::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	// Retrieve the AIController from the owner
	ANGN_GnomeController* CharPC = Cast<ANGN_GnomeController>(OwnerComp.GetAIOwner());
	if (CharPC) {

		// Retrieve the blackboard from the ownerAI
		UBlackboardComponent* blackboard = CharPC->GetBlackboardComponent();

		// Retrieve the steering manager from the owner
		TArray<USteeringManager*> steManagerComps;
		CharPC->GetOwner()->GetComponents<USteeringManager>(steManagerComps);

		if (blackboard && steManagerComps.Num() > 0) {

			// Retrieve the target from the blackboard
			FName keyID = GetSelectedBlackboardKey();
			//FName keyID = "EnemyActor";
			AActor* target = Cast<AActor>(blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID(keyID)));
			
			if (target) {

				// Retrieve location and velocity of the target
				FVector targetLocation = target->GetTransform().GetLocation();
				FVector targetVelocity = { 0.f, 0.f, 0.f};

				// Apply the configured behaviour
				USteeringManager* steManager = steManagerComps[0];
				switch (steeringToApply) {

					// This methods sets to zero the force to apply on the steeringManager owner
					case EBTSteeringType::reset: steManager->reset(); break;
					
					// This methods update the force to apply on the owner
					case EBTSteeringType::seek: steManager->seek(targetLocation); break;
					case EBTSteeringType::flee: steManager->flee(targetLocation); break;
					case EBTSteeringType::arrival: steManager->arrival(targetLocation); break;
					case EBTSteeringType::pursuit: steManager->pursuit(targetLocation, targetVelocity); break;
					case EBTSteeringType::evade: steManager->evade(targetLocation, targetVelocity); break;

					case EBTSteeringType::wander: steManager->wander(); break;
					case EBTSteeringType::flock: steManager->flocking(); break;

					// This method is the one to call to update the owner's position
					case EBTSteeringType::update: 
						FVector nextPosition = steManager->update();
						CharPC->MoveToLocation(nextPosition, 0.f, true, false, true, true, 0, true);
						break;
				}

				// Return success
				return EBTNodeResult::Succeeded;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("There is no %s"), "target assigned");
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("There is no %s"), "blackboard or steeringManager component assigned to the actor");
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("There is no %s"), "CharPC");
	}
	// Return failure
	return EBTNodeResult::Failed;
}