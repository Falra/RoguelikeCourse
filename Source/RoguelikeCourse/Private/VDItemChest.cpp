// Fill out your copyright notice in the Description page of Project Settings.


#include "VDItemChest.h"
#include "Net/UnrealNetwork.h"

AVDItemChest::AVDItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	SetReplicates(true);
}

void AVDItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	float CurrentPitch = bLidOpened ? TargetPitch : 0.f;
	
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
	
}

void AVDItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVDItemChest, bLidOpened);
}

