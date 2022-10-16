// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EnvironmentQuery/EnvQueryTypes.h"

#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Kill")
	void OnActorKilled(AActor* VictimActor, AActor* Killer);

protected:

	FTimerHandle SpawnBotsTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UFUNCTION()
	void SpawnBot_TimeElapsed();

	UFUNCTION()
	void RespawnPlayer_TimeElapsed(AController* Controller);

	UFUNCTION()
	void OnCompletedQuery(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float PlayerReSpawnTime;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 KillCredits;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(Exec)
	void ToggleAIMove();
	
	bool bCanMove;
};
