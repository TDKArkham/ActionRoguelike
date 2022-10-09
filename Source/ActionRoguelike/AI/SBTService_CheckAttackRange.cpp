// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		APawn* TargetPawn = Cast<APawn>(Blackboard->GetValueAsObject("TargetActor"));
		if(TargetPawn)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (AIController)
			{
				float Distance = FVector::Distance(TargetPawn->GetActorLocation(), AIController->GetPawn()->GetActorLocation());
				bool bWithinRange = Distance < 2000.0f;

				bool bHasLOS = false;
				if(bWithinRange)
				{
					bHasLOS = AIController->LineOfSightTo(TargetPawn);
				}
				 
				Blackboard->SetValueAsBool(KeySelector.SelectedKeyName, (bWithinRange && bHasLOS));
			}
		}
	}
}
