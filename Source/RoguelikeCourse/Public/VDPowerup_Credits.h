// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDPowerupActor.h"
#include "VDPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API AVDPowerup_Credits : public AVDPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	AVDPowerup_Credits();	
};
