// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()
public:
	explicit USBTService_CheckLowHealth()
		: LowHealth(0.4)
	{
	}

protected:

	UPROPERTY(EditAnywhere, Category = "Health", DisplayName = "LowHealthProportion", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float LowHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	FBlackboardKeySelector KeySelector;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
