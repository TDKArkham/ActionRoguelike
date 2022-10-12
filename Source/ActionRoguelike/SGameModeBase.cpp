// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsTimerHandle, this, &ASGameModeBase::SpawnBot_TimeElapsed, SpawnTimeInterval, true);
}

void ASGameModeBase::SpawnBot_TimeElapsed()
{
	/****************Check If the World Can Hold More AICharacters************/
	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponnent(Bot);
		if (AttributeComponent && USAttributeComponent::GetActorAlive(Bot))
		{
			NumOfAliveBots++;
		}
	}
	float MaxBotNum = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotNum = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NumOfAliveBots >= MaxBotNum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Maximum Bots!"));
		return;;
	}
	/****************************Check Finished******************************/

	
	/*******************************Run EQS******************************/
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnCompletedQuery);
	}
}

void ASGameModeBase::OnCompletedQuery(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponnent(Bot);
		if (AttributeComponent && USAttributeComponent::GetActorAlive(Bot))
		{
			AttributeComponent->Kill(this);
		}
	}
}
