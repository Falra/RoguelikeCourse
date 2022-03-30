// Fill out your copyright notice in the Description page of Project Settings.


#include "VDPlayerState.h"

#include "VDSaveGame.h"

void AVDPlayerState::AddCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AVDPlayerState::RemoveCredits(int32 Delta)
{
	// Avoid user-error of adding a subtracting negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// Not enough credits available
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

void AVDPlayerState::SavePlayerState_Implementation(UVDSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AVDPlayerState::LoadPlayerState_Implementation(UVDSaveGame* SaveObject)
{
	if(SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}

int32 AVDPlayerState::GetCredits() const
{
	return Credits;
}
