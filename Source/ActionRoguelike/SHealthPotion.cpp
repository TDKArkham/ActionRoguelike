﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn);
	if (Player)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Player->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->GetHealth() < 100.0f)
		{
			AttributeComp->ApplyHealthChange(HealPower);
			DeActivateActor();
		}
	}
}