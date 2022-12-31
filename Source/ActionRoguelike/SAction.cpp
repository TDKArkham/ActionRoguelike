// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "ActionRoguelike.h"
#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

void USAction::StartAction_Implementation(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Action: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* ActionComp = GetOwnerComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.TargetActor = TargetActor;
}

void USAction::StopAction_Implementation(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Action: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	USActionComponent* ActionComp = GetOwnerComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.TargetActor = TargetActor;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(RepData.bIsRunning)
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
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwnerComponent() const
{
	return ActionComponent;
}

void USAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.TargetActor);
	}
	else
	{
		StopAction(RepData.TargetActor);
	}
}

bool USAction::GetIsRunning()
{
	return RepData.bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComponent);
}