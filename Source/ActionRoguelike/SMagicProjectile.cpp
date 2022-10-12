// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetCollisionProfileName("Projectile");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->InitialSpeed = 1000.0f;
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bInitialVelocityInLocalSpace = true;

	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	FlightAudioComponent->SetupAttachment(RootComponent);

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);

	if (FlightCue)
	{
		FlightAudioComponent->SetSound(FlightCue);
		FlightAudioComponent->Play();
	}
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		if(ImpactCue)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactCue, GetActorLocation(), GetActorRotation());
		}
		if (ExplodeParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticle, GetActorLocation(), GetActorRotation());
		}
		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(GetInstigator(), -Damage);

			Destroy();
		}
	}
}
