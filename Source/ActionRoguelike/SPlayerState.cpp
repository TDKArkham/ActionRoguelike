// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"


void ASPlayerState::AddCredits(int32 Delta)
{
	if (Delta <= 0)
	{
		return;
	}
	Credits += Delta;
	OnCreditChange.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredit(int32 Delta)
{
	if (Delta <= 0 || Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditChange.Broadcast(this, Credits, -Delta);
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}
