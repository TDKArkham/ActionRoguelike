// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_Effect.h"
#include "SAction_Effect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Effect_Thorns : public USAction_Effect
{
	GENERATED_BODY()

public:
	USAction_Effect_Thorns();
	
	virtual void StartAction_Implementation(AActor* TargetActor) override;
	virtual void StopAction_Implementation(AActor* TargetActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action", meta=(ClampMin = 0, ClampMax = 1))
	float ReflectFraction;
	
protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, class USAttributeComponent* OwnerComponent, float NewHealth, float Delta);
};
