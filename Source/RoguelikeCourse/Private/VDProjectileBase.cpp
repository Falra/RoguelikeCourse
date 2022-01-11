// Fill out your copyright notice in the Description page of Project Settings.


#include "VDProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVDProjectileBase::AVDProjectileBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentHit.AddDynamic(this, &AVDProjectileBase::OnActorHit);
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(RootComponent);

	MoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComponent");
	MoveComponent->bRotationFollowsVelocity = true;
	MoveComponent->bInitialVelocityInLocalSpace = true;
	MoveComponent->ProjectileGravityScale = 0.0f;
	MoveComponent->InitialSpeed = 8000;
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