// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPlayerController.h"

void AVDPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void AVDPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}


void AVDPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}