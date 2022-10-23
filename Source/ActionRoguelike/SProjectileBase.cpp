// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionProfileName("Projectile");
	RootComponent = SphereCollision;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->InitialSpeed = 2000.0f;
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bInitialVelocityInLocalSpace = true;

	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	FlightAudioComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitExplode();
}

void ASProjectileBase::HitExplode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		if(ExplodeParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticle, GetActorLocation(), GetActorRotation());
		}
		
		if(ImpactCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactCue, GetActorLocation());
		}
		
		Destroy();
	}
}

