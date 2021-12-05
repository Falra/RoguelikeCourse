// Fill out your copyright notice in the Description page of Project Settings.


#include "VDItemChest.h"

void AVDItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

// Sets default values
AVDItemChest::AVDItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

}

// Called when the game starts or when spawned
void AVDItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

