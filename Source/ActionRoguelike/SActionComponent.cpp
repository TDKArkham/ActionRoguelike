
#include "SActionComponent.h"

#include "SAction.h"


USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		if (ActionClass)
		{
			AddAction(ActionClass);
		}
	}
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ActionClass)
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if (NewAction)
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (!Action->CanStart(Instigator))
		{
			continue;
		}
		if (Action && Action->ActionName == ActionName)
		{
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
		if(Action->GetIsRunning())
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
