// Fill out your copyright notice in the Description page of Project Settings.


#include "VDMagicProjectile.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AVDMagicProjectile::AVDMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	//SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionProfileName("Projectile");

	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	

}

// Called when the game starts or when spawned
void AVDMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

