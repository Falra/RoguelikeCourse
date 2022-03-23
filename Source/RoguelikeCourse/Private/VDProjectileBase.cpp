// Fill out your copyright notice in the Description page of Project Settings.


#include "VDProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AVDProjectileBase::AVDProjectileBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentHit.AddDynamic(this, &AVDProjectileBase::OnActorHit);
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	MoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComponent");
	MoveComponent->bRotationFollowsVelocity = true;
	MoveComponent->bInitialVelocityInLocalSpace = true;
	MoveComponent->ProjectileGravityScale = 0.0f;
	MoveComponent->InitialSpeed = 8000;

	ImpactShakeInnerRadius = 250.0f;
	ImpactShakeOuterRadius = 2500.0f;

	SetReplicates(true);
}

void AVDProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

// _Implementation from it being marked as BlueprintNativeEvent
void AVDProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		
		EffectComponent->DeactivateSystem();

		MoveComponent->StopMovementImmediately();
		SetActorEnableCollision(false);

		Destroy();
	}
}

void AVDProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}