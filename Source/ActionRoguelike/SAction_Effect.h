// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_Effect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Effect : public USAction
{
	GENERATED_BODY()
public:
	USAction_Effect();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle DurationTimerHandle;
	FTimerHandle PeriodTimerHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
	
public:
	virtual void StartAction_Implementation(AActor* TargetActor) override;
	virtual void StopAction_Implementation(AActor* TargetActor) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRamaining() const;
};
