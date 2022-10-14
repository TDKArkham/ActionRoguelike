// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SGameplayInterface.h"
#include "SHealthPotion.h"
#include "SInteractionComponent.h"
#include "SMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::OnComponentBeginOverlap);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryBlackhole", IE_Pressed, this, &ASCharacter::PrimaryBlackhole);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;
	FVector ControlVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ControlVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	

	const FLatentActionInfo LatentActionInfo(0, FMath::Rand(), TEXT("PrimaryAttack_TimeElapsed"), this);
	UKismetSystemLibrary::Delay(this, 0.17f, LatentActionInfo);

	//GetWorldTimerManager().SetTimer(TimerHandle_ShootDelay, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.17f);

}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Shoot!"));
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector HitPosition = GetHitLocation();
	FTransform SpawnTM = FTransform(FRotationMatrix::MakeFromX(HitPosition - HandLocation).Rotator(), HandLocation);
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.Instigator = this;

	if (ProjectileClass)
	{
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParam);
	}
}

void ASCharacter::PrimaryBlackhole()
{
	PlayAnimMontage(AttackAnim);

	const FLatentActionInfo LatentActionInfo(0, FMath::Rand(), TEXT("PrimaryBlackhole_TimeElapsed"), this);
	UKismetSystemLibrary::Delay(this, 0.17f, LatentActionInfo);
}

void ASCharacter::PrimaryBlackhole_TimeElapsed()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Shoot!"));
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector HitPosition = GetHitLocation();
	FTransform SpawnTM = FTransform(FRotationMatrix::MakeFromX(HitPosition - HandLocation).Rotator(), HandLocation);
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.Instigator = this;

	if (ProjectileClass)
	{
		GetWorld()->SpawnActor<AActor>(BlackHoleClass, SpawnTM, SpawnParam);
	}
}

FVector ASCharacter::GetHitLocation()
{
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = GetControlRotation().Vector() * 2000.0f + Start;	//GetControlRotation(), Better than CameraComponent->GetComponentRotation()
	FHitResult Hit;
	
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic);
	if (bIsHit)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f);
		return Hit.ImpactPoint;
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		return End;
	}
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwnerComponent, float NewHealth, float Delta)
{
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		DisableInput(PC);
	}
}

void ASCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<USGameplayInterface>())
	{
		ISGameplayInterface::Execute_Interact(OtherActor, this);
	}
}

void ASCharacter::HealSelf(float Amount /* = 100.0f */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}
