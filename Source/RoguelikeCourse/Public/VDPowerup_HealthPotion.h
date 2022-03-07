// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDPowerupActor.h"
#include "VDPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API AVDPowerup_HealthPotion : public AVDPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "HealthPotion")
	int32 CreditCost;

	// float healt amount?

	public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	AVDPowerup_HealthPotion();	
};
