// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class USActionComponent;

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetIsRunning();

	UFUNCTION(BlueprintCallable, Category = "Tags")
	USActionComponent* GetOwnerComponent();
	
	virtual UWorld* GetWorld() const override;
};
