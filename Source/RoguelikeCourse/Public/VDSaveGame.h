// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VDSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API UVDSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Credits;
};
