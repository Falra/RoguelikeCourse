// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAttributeComponent.h"

UVDAttributeComponent::UVDAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UVDAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta); // @fixme: Still nullptr for InstigatorActor parameter

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

