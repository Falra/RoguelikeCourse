// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAttributeComponent.h"

// Sets default values for this component's properties
UVDAttributeComponent::UVDAttributeComponent()
{
	Health = 100;
}
bool UVDAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	Health += DeltaHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
	
	return true;
}

bool UVDAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

