// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDAICharacter.h"

// Sets default values
AVDAICharacter::AVDAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVDAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVDAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
