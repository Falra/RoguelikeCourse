// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VDSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/* Identifier which actor this belongs to */
	UPROPERTY()
	FString ActorName;

	/* For movable actors keep location, rotation, scale. */
	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> ByteData;
};


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

	UPROPERTY()
	TArray<FActorSaveData> SaveActors;
};
