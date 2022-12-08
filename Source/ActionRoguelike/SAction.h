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

	UPROPERTY(Replicated)
	USActionComponent* ActionComponent;
	
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();
	
public:

	void Initialize(USActionComponent* NewActionComponent);
	
	bool IsSupportedForNetworking() const override { return true; }
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetIsRunning();

	UFUNCTION(BlueprintCallable, Category = "Tags")
	USActionComponent* GetOwnerComponent() const;
	
	virtual UWorld* GetWorld() const override;
};
