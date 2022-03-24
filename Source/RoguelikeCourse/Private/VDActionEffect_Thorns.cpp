// Fill out your copyright notice in the Description page of Project Settings.


#include "VDActionEffect_Thorns.h"

#include "VDActionComponent.h"
#include "VDAttributeComponent.h"
#include "VDGameplayFunctionLibrary.h"

UVDActionEffect_Thorns::UVDActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UVDActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UVDAttributeComponent* Attributes = UVDAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UVDActionEffect_Thorns::OnHealthChanged);
	}
}

void UVDActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UVDAttributeComponent* Attributes = UVDAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UVDActionEffect_Thorns::OnHealthChanged);
	}
}

void UVDActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComp,
                                             float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		UVDGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}
