// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EnvironmentQuery/EnvQueryTypes.h"

#include "SGameModeBase.generated.h"

class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "Kill")
	void OnActorKilled(AActor* VictimActor, AActor* Killer);

protected:

	FTimerHandle SpawnBotsTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	
	/**
	 * @brief AI Spawn Time Interval
	 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float PlayerRespawnTime;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 KillCredits;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	FString SlotName;

	bool bCanMove;

	UFUNCTION()
	void SpawnBot_TimeElapsed();

	UFUNCTION()
	void RespawnPlayer_TimeElapsed(AController* Controller);

	UFUNCTION()
	void OnCompletedQuery(class UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(Exec)
	void ToggleAIMove();
};
