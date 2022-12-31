// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100.0f;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100.0f;

	SetIsReplicatedByDefault(true);
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

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigateActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float TrueDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		// Only do this when in server.
		Health = NewHealth;
		if (TrueDelta != 0.0f)
		{
			MulticastHealthChanged(InstigateActor, Health, TrueDelta);
		}

		if (TrueDelta < 0 && Health == 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigateActor);
			}
		}
	}

	return TrueDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* TargetActor)
{
	if (TargetActor)
	{
		return Cast<USAttributeComponent>(TargetActor->GetComponentByClass(StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::GetActorAlive(AActor* TargetActor)
{
	if (TargetActor)
	{
		return GetAttributeComponent(TargetActor)->GetIsAlive();
	}
	return false;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigateActor, float Delta)
{
	float OldRage = Rage;
	Rage += Delta;
	Rage = FMath::Clamp(Rage, 0.0f, RageMax);

	float TrueDelta = Rage - OldRage;

	if (TrueDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigateActor, this, Rage, TrueDelta);
		//MulticastHealthChanged(InstigateActor, Health, TrueDelta);
	}

	/*if (TrueDelta < 0 && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigateActor);
		}
	}*/

	return TrueDelta != 0;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);

	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_OwnerOnly);
}