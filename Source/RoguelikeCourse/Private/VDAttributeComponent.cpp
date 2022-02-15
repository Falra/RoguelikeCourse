// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAttributeComponent.h"

#include "VDGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("vd.DamageMultiplier"), 1.0f,
	TEXT("Global damage multipier for attribute component"), ECVF_Cheat);

UVDAttributeComponent::UVDAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UVDAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	if(!GetOwner()->CanBeDamaged() && DeltaHealth < 0.0f)
	{
		return false;
	}

	if(DeltaHealth < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		DeltaHealth *= DamageMultiplier;
	}
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if(ActualDelta <= 0.0f && Health == 0.0f)
	{
		AVDGameModeBase* GM = GetWorld()->GetAuthGameMode<AVDGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

bool UVDAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UVDAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UVDAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float UVDAttributeComponent::GetHealth() const
{
	return Health;
}

bool UVDAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

UVDAttributeComponent* UVDAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UVDAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

bool UVDAttributeComponent::IsActorAlive(AActor* Actor)
{
	UVDAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if(AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}
