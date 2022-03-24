// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDPowerupActor.h"
#include "VDPowerup_Action.generated.h"

class UVDAction;
/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API AVDPowerup_Action : public AVDPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UVDAction> ActionToGrant;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;
};
