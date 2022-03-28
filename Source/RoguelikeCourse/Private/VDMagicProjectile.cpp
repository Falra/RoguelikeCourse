// Fill out your copyright notice in the Description page of Project Settings.


#include "VDMagicProjectile.h"

#include "VDActionComponent.h"
#include "VDActionEffect.h"
#include "VDAttributeComponent.h"
#include "VDGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
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
		/*
		UVDAttributeComponent* AttributeComponent = UVDAttributeComponent::GetAttributes(OtherActor);
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(GetInstigator(), -DamageAmount);
			Explode();
		}
		*/

		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying"); 
		
		UVDActionComponent* ActionComponent = OtherActor->FindComponentByClass<UVDActionComponent>();
		if(ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComponent->Velocity = - MoveComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}


		// Apply damage & impulse
		if (UVDGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if(ActionComponent)
			{
				ActionComponent->AddAction(GetInstigator(), ActionEffectClass);
			}
		}
	}
}
