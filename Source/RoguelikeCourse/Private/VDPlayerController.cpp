// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPlayerController.h"

#include "Blueprint/UserWidget.h"

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

void AVDPlayerController::TogglePauseMenu()
{
	if(PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AVDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AVDPlayerController::TogglePauseMenu);
}
