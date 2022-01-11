// Fill out your copyright notice in the Description page of Project Settings.


#include "VDMagicProjectile.h"
#include "VDAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AVDMagicProjectile::AVDMagicProjectile()
{
	SphereComponent->SetSphereRadius(20.0f);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AVDMagicProjectile::OnBeginOverlap);

	DamageAmount = 20.0f;
}

void AVDMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UVDAttributeComponent* AttributeComponent = Cast<UVDAttributeComponent>(OtherActor->GetComponentByClass(UVDAttributeComponent::StaticClass()));
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-DamageAmount);
			Explode();
		}
	}
}
