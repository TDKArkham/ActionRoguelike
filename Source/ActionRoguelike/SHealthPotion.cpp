// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreditCost = 50;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(InstigatorPawn);
	if (AttributeComp && AttributeComp->GetHealth() < 100.0f)
	{
		ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();

		if (PS && PS->RemoveCredit(CreditCost) && AttributeComp->ApplyHealthChange(this, HealPower))
		{
			DeActivateActor();
		}
	}
}
