// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (AIC && BB)
	{
		APawn* ControlledPawn = AIC->GetPawn();
		if (ControlledPawn)
		{
			USAttributeComponent* Attribute = USAttributeComponent::GetAttributeComponnent(ControlledPawn);
			if(Attribute)
			{
				BB->SetValueAsBool("bIsLowHealth", Attribute->GetHealth() <= LowHealthStandard);
			}
		}
	}
}
