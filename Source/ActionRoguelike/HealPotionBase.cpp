// Fill out your copyright notice in the Description page of Project Settings.


#include "HealPotionBase.h"

#include "SCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
AHealPotionBase::AHealPotionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);

	HealPower = 10.0f;
	CooldownTime = 10.0f;
}

void AHealPotionBase::ActivateActor()
{
	RootComponent->SetVisibility(true, true);
	SetActorEnableCollision(true);
	GetWorldTimerManager().ClearTimer(DeactivateHandle);
}

void AHealPotionBase::DeActivateActor()
{
	RootComponent->SetVisibility(false, true);
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(DeactivateHandle, this, &AHealPotionBase::ActivateActor, CooldownTime);
}

void AHealPotionBase::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	//Leave to be implemented by derived class
}

