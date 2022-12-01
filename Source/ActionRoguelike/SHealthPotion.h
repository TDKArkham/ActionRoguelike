// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableItemBase.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickableItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASHealthPotion();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Credits")
	int32 CreditCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal")
	float HealPower;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
