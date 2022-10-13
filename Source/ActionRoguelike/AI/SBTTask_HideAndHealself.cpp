// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_HideAndHealself.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_HideAndHealself::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (AIC && BB)
	{
		APawn* ControlledPawn = AIC->GetPawn();
		if (ControlledPawn)
		{
			USAttributeComponent* Attribute = USAttributeComponent::GetAttributeComponnent(ControlledPawn);
			if (Attribute->GetHealth() < 100.f)
			{
				Attribute->ApplyHealthChange(nullptr, HealPerSecond);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
