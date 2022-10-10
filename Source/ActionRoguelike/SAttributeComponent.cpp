﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100.0f;
	Health = HealthMax;
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USAttributeComponent::GetIsAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Health-OldHealth);

	return true;
}