// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_HideAndHeal.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_HideAndHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AActor* OwnerPawn = AIC->GetPawn();
		if (OwnerPawn)
		{
			USAttributeComponent* Attribute = USAttributeComponent::GetAttributeComponent(OwnerPawn);
			if (Attribute)
			{
				Attribute->ApplyHealthChange(nullptr, HealHealth);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
