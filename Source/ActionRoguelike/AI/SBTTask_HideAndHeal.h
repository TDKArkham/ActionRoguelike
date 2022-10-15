// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_HideAndHeal.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTask_HideAndHeal : public UBTTaskNode
{
	GENERATED_BODY()
public:
	explicit USBTTask_HideAndHeal()
		: HealHealth(50)
	{
	}

protected:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
	float HealHealth;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
