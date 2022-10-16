// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableItemBase.h"
#include "SCredits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCredits : public ASPickableItemBase
{
	GENERATED_BODY()
public:
	explicit ASCredits()
		: CreditAmount(80)
	{
	}

protected:
	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditAmount;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
