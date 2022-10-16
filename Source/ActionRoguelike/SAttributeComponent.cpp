// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100.0f;
	Health = HealthMax;
}


bool USAttributeComponent::GetIsAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigateActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	float TrueDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigateActor, this, Health, TrueDelta);

	if( TrueDelta < 0 && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigateActor);
		}
	}

	return TrueDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* TargetActor)
{
	if(TargetActor)
	{
		return Cast<USAttributeComponent>(TargetActor->GetComponentByClass(StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::GetActorAlive(AActor* TargetActor)
{
	if(TargetActor)
	{
		return GetAttributeComponent(TargetActor)->GetIsAlive();
	}
	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}
