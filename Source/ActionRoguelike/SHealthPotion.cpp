// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);

	HealPower = 10.0f;
}

// Called when the game starts or when spawned
void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHealthPotion::Active()
{
	MeshComponent->SetVisibility(true);
	BoxTrigger->SetGenerateOverlapEvents(true);
	GetWorldTimerManager().ClearTimer(DeactivateHandle);
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn);
	if (Player)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Player->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->GetHealth() < 100.0f)
		{
			AttributeComp->ApplyHealthChange(HealPower);
			MeshComponent->SetVisibility(false);
			BoxTrigger->SetGenerateOverlapEvents(false);
			GetWorldTimerManager().SetTimer(DeactivateHandle, this, &ASHealthPotion::Active, 10.0f);
		}
	}
}
