// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Action: %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Action: %s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const
{
	// Outer: set when create the action in SActionComponent via NewObject<T>
	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	if (ActorComponent)
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}
