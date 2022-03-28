// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPlayerController.h"

void AVDPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
