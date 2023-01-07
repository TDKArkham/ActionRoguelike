// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableItemBase.h"
#include "SPowerup_Action.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Action : public ASPickableItemBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<USAction> ActionToGrant;
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
