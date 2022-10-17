// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("DrawDebug"), false, TEXT("Toggle to draw debug things in game"));

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 1000.0f;
	TraceChannel = ECC_WorldDynamic;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDrawDebug.GetValueOnGameThread();
	
	AActor* MyOwner = GetOwner();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceChannel);

	FHitResult Hit;

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	//FVector Start;
	FVector End = EyeLocation + EyeRotation.Vector() * TraceDistance;

	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	FocusedActor = nullptr;
	
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if(bDrawDebug)
		{
			DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 2.0f);
		}
		
		if (HitActor->Implements<USGameplayInterface>())
		{
			FocusedActor = HitActor;
		}
	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && DefaultWidgetClass)
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void USInteractionComponent::PrimaryInteract()
{
	if(FocusedActor == nullptr)
	{
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
