// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Taggole SpawnBots Via Timer"));

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
	PlayerReSpawnTime = 2.0f;
	KillCredits = 100;
	bCanMove = true;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsTimerHandle, this, &ASGameModeBase::SpawnBot_TimeElapsed, SpawnTimeInterval, true);
}

void ASGameModeBase::SpawnBot_TimeElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		return;
	}
	
	/****************Check If the World Can Hold More AICharacters************/
	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponent(Bot);
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

void ASGameModeBase::RespawnPlayer_TimeElapsed(AController* Controller)
{
	if(Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayer_TimeElapsed", Player->GetController());
		FTimerHandle RespawnPlayerTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnPlayerTimerHandle, Delegate, PlayerReSpawnTime, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if(KillerPawn)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(KillerPawn->GetPlayerState());
		if(PS)
		{
			PS->AddCredits(KillCredits);
		}
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponent(Bot);
		if (AttributeComponent && USAttributeComponent::GetActorAlive(Bot))
		{
			AttributeComponent->Kill(this);
		}
	}
}

void ASGameModeBase::ToggleAIMove()
{
	bCanMove = !bCanMove;
	if(bCanMove)
	{
		for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
		{
			ASAICharacter* Bot = *It;

			AAIController* AIC = Cast<AAIController>(Bot->GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StartLogic();
			}
		}
	}
	else
	{
		for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
		{
			ASAICharacter* Bot = *It;

			AAIController* AIC = Cast<AAIController>(Bot->GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Cheat Code Activated!");
			}
		}
	}
}
