// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "SActionComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Taggole SpawnBots Via Timer"));

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
	PlayerRespawnTime = 2.0f;
	KillCredits = 100;
	bCanMove = true;

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsTimerHandle, this, &ASGameModeBase::SpawnBot_TimeElapsed, SpawnTimeInterval, true);
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

void ASGameModeBase::SpawnBot_TimeElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
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
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnCompletedQuery);
	}
}

void ASGameModeBase::OnCompletedQuery(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		if (MonstersTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonstersTable->GetAllRows("", Rows);

			int32 Index = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* Row = Rows[Index];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				TArray<FName> Bundles;

				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, Row->MonsterID, Locations[0]);

				Manager->LoadPrimaryAsset(Row->MonsterID, Bundles, Delegate);
			}


		}

	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId AssetId, FVector SpawnLocation)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(AssetId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				USActionComponent* ActionComponent = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
				if (ActionComponent)
				{
					for (TSubclassOf<USAction> Action : MonsterData->Actions)
					{
						ActionComponent->AddAction(NewBot, Action);
					}
				}
			}
		}
	}


}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayer_TimeElapsed", Player->GetController());
		FTimerHandle RespawnPlayerTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnPlayerTimerHandle, Delegate, PlayerRespawnTime, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(KillerPawn->GetPlayerState());
		if (PS)
		{
			PS->AddCredits(KillCredits);
		}
	}
}

void ASGameModeBase::RespawnPlayer_TimeElapsed(AController* Controller)
{
	if (Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;	// Now Only Single Player
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator it(GetWorld()); it; ++it)
	{
		AActor* Actor = *it;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemoryWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		// Only find variables that has "SaveGame" specifier in UPROPERTY() micro
		Ar.ArIsSaveGame = true;
		// Convert Actor's "SaveGame" UPROPERTY into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data!"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data!"));

		for (FActorIterator it(GetWorld()); it; ++it)
		{
			AActor* Actor = *it;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemoryReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
					// Only find variables that has "SaveGame" specifier in UPROPERTY() micro
					Ar.ArIsSaveGame = true;
					// Convert binary data into Actor's "SaveGame" UPROPERTY
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame Data!"))
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
	if (bCanMove)
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
