// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "ActionRoguelike/SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwnerComponent, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(UserWidget == nullptr)
		{
			UserWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if(UserWidget)
			{
				UserWidget->AttachedActor = this;
				UserWidget->AddToViewport();
			}
		}

		if (NewHealth <= 0.0f)
		{
			//Disable Behavior Tree
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Got Killed");
			}

			//Set Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//SetLifeSpan, Destroy self after 10 sec
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* Actor)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);
}
