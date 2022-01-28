// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "VDPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class ROGUELIKECOURSE_API AVDPowerupActor : public AActor, public IVDGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVDPowerupActor();

protected:
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

public:	
	void Interact_Implementation(APawn* InstigatorPawn) override;

};
