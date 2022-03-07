// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPowerup_HealthPotion.h"

#include "VDAttributeComponent.h"
#include "VDPlayerState.h"

AVDPowerup_HealthPotion::AVDPowerup_HealthPotion()
{
	CreditCost = 50;
}

void AVDPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UVDAttributeComponent* AttributeComp = UVDAttributeComponent::GetAttributes(InstigatorPawn);
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AVDPlayerState* PS = InstigatorPawn->GetPlayerState<AVDPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				// Only activate if healed successfully
				HideAndCooldownPowerup();
			}
		}
	}
}
