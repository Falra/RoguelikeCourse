// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAttributeComponent.h"

#include "VDGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("vd.DamageMultiplier"), 1.0f,
                                                        TEXT("Global damage multipier for attribute component"), ECVF_Cheat);

UVDAttributeComponent::UVDAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;
	
	SetIsReplicatedByDefault(true);
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
	float NewHealth = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;
	
	// Is Server?
	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
		if(ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, NewHealth, ActualDelta);
		}

		// Died
		if(ActualDelta <= 0.0f && NewHealth == 0.0f)
		{
			AVDGameModeBase* GM = GetWorld()->GetAuthGameMode<AVDGameModeBase>();
			if(GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

float UVDAttributeComponent::GetRage() const
{
	return Rage;
}

bool UVDAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
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

void UVDAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
                                                                  float DeltaHealth)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, DeltaHealth);
}

void UVDAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UVDAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVDAttributeComponent, Health);
	DOREPLIFETIME(UVDAttributeComponent, HealthMax);
	// DOREPLIFETIME_CONDITION(UVDAttributeComponent, HealthMax, COND_InitialOnly);

	DOREPLIFETIME(UVDAttributeComponent, Rage);
	DOREPLIFETIME(UVDAttributeComponent, RageMax);
}
