// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Action: %s"), *GetNameSafe(this));

	USActionComponent* ActionComp = GetOwnerComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Action: %s"), *GetNameSafe(this));

	USActionComponent* ActionComp = GetOwnerComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(bIsRunning)
	{
		return false;
	}
	
	USActionComponent* ActionComp = GetOwnerComponent();
	if(ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
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

USActionComponent* USAction::GetOwnerComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::GetIsRunning()
{
	return bIsRunning;
}
