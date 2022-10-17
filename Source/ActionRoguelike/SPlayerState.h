// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class ASPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditChange, ASPlayerState*, PlayerState, int32, NewCredit, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;

public:

	UPROPERTY(BlueprintAssignable)
	FOnCreditChange OnCreditChange;
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredit(int32 Delta);
};