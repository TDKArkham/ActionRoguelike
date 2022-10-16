// Fill out your copyright notice in the Description page of Project Settings.


#include "SCredits.h"

#include "SPlayerState.h"

void ASCredits::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(InstigatorPawn)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if(PS)
		{
			PS->AddCredits(CreditAmount);
			DeActivateActor();
		}
	}
}
