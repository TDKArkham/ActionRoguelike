// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_HideAndHealself.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTask_HideAndHealself : public UBTTaskNode
{
	GENERATED_BODY()
public:
	explicit USBTTask_HideAndHealself()
		: HealPerSecond(100)
	{
	}
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealPerSecond ;
};
