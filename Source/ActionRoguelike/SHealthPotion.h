// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealPotionBase.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public AHealPotionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASHealthPotion();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
