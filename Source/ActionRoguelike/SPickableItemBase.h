// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickableItemBase.generated.h"

class UBoxComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPickableItemBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPickableItemBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal")
	float HealPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal")
	float CooldownTime;

private:

	FTimerHandle DeactivateHandle;		//Handle the Delay of HealPotion after "used" by player

public:

	void ActivateActor();
	void DeActivateActor();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
