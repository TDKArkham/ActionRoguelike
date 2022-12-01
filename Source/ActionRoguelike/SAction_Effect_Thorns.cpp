// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Effect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USAction_Effect_Thorns::USAction_Effect_Thorns()
{
	ReflectFraction = 0.2;
}

void USAction_Effect_Thorns::StartAction_Implementation(AActor* TargetActor)
{
	Super::StartAction_Implementation(TargetActor);

	USAttributeComponent* Attribute = USAttributeComponent::GetAttributeComponent(GetOwnerComponent()->GetOwner());
	if(Attribute)
	{
		Attribute->OnHealthChanged.AddDynamic(this, &USAction_Effect_Thorns::OnHealthChanged);
	}
}

void USAction_Effect_Thorns::StopAction_Implementation(AActor* TargetActor)
{
	Super::StopAction_Implementation(TargetActor);
	
	USAttributeComponent* Attribute = USAttributeComponent::GetAttributeComponent(GetOwnerComponent()->GetOwner());
	if (Attribute)
	{
		Attribute->OnHealthChanged.RemoveDynamic(this, &USAction_Effect_Thorns::OnHealthChanged);
	}
}

void USAction_Effect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwnerComponent, float NewHealth, float Delta)
{
	AActor* OwnerActor = GetOwnerComponent()->GetOwner();

	if(Delta < 0.0f && OwnerActor != InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if(ReflectedAmount == 0)
		{
			return;
		}

		ReflectedAmount = FMath::Abs(ReflectedAmount);

		USGameplayFunctionLibrary::ApplyDamage(OwnerActor, InstigatorActor, ReflectedAmount);
	}
}
