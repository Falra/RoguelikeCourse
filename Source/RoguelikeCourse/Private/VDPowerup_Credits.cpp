// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPowerup_Credits.h"
#include "VDPlayerState.h"

AVDPowerup_Credits::AVDPowerup_Credits()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditsAmount = 80;
}

void AVDPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AVDPlayerState* PS = InstigatorPawn->GetPlayerState<AVDPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}
