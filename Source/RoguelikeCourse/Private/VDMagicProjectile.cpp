// Fill out your copyright notice in the Description page of Project Settings.


#include "VDMagicProjectile.h"

#include "VDAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AVDMagicProjectile::AVDMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	//SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AVDMagicProjectile::OnBeginOverlap);

	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	

}

void AVDMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UVDAttributeComponent* AttributeComponent = Cast<UVDAttributeComponent>(OtherActor->GetComponentByClass(UVDAttributeComponent::StaticClass()));
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
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

