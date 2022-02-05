// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPowerupActor.h"

#include "Components/SphereComponent.h"

AVDPowerupActor::AVDPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
}

void AVDPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}


void AVDPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}


void AVDPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	FTimerHandle TimerHandle_RespawnTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AVDPowerupActor::ShowPowerup, RespawnTime);
}

void AVDPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	// Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}