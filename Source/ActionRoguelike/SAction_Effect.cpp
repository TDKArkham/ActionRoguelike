// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Effect.h"

#include "SActionComponent.h"

USAction_Effect::USAction_Effect()
{
	bAutoStart = true;
}

void USAction_Effect::StartAction_Implementation(AActor* TargetActor)
{
	Super::StartAction_Implementation(TargetActor);

	if(Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", TargetActor);
		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", TargetActor);
		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, Delegate, Period, true);
	}
}

void USAction_Effect::StopAction_Implementation(AActor* TargetActor)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimerHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(TargetActor);
	}
	
	Super::StopAction_Implementation(TargetActor);

	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);

	USActionComponent* ActionComp = GetOwnerComponent();
	if(ActionComp)
	{
		ActionComp->RemoveAction(this);
	}
}

void USAction_Effect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}