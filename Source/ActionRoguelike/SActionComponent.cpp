
#include "SActionComponent.h"

#include "ActionRoguelike.h"
#include "SAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			if (ActionClass)
			{
				AddAction(GetOwner(), ActionClass);
			}
		}
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (USAction* Action : Actions)
	{
		FColor TextColor = Action->GetIsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->GetIsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(Action->GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ActionClass)
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);

	if (NewAction)
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);

		if (NewAction->CanStart(Instigator) && NewAction->bAutoStart)
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!(ActionToRemove && !ActionToRemove->GetIsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

USAction* USActionComponent::HasAction(TSubclassOf<USAction> ActionToCheck)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionToCheck))
		{
			return Action;
		}
	}

	return nullptr;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action->GetIsRunning())
		{
			if (Action && Action->ActionName == ActionName)
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}
