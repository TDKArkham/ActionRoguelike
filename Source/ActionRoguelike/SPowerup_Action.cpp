// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Action.h"

#include "SAction.h"
#include "SActionComponent.h"


void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(!(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent* ActionComponent = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if(ActionComponent)
	{
		if (ActionComponent->HasAction(ActionToGrant))
		{
			return;
		}

		ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
		DeActivateActor();
	}
}

