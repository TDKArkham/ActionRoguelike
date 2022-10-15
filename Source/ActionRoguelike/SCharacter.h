// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual FVector GetPawnViewLocation() const override;

protected:

	/***********************************Components************************************/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USActionComponent* ActionComponent;

	/***********************************Sub - Assets************************************/
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	/***********************************Functions************************************/
	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void PrimaryAttack();

	void PrimaryInteract();

	void SprintStart();
	
	void SprintStop();
	
	UFUNCTION()
	void PrimaryAttack_TimeElapsed();

	void PrimaryBlackhole();
	
	UFUNCTION()
	void PrimaryBlackhole_TimeElapsed();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, class USAttributeComponent* OwnerComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/***********************************Console Commands************************************/
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);
	
private:
	FVector GetHitLocation();
	
};
