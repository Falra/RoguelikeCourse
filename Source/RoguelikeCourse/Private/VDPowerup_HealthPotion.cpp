// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPowerup_HealthPotion.h"

#include "VDAttributeComponent.h"
#include "VDPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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

FText AVDPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UVDAttributeComponent* AttributeComp = UVDAttributeComponent::GetAttributes(InstigatorPawn);
	if(AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}
	
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage",
			"Cost {0} Credits. Restores health to maximum."), CreditCost);
}

#undef LOCTEXT_NAMESPACE
