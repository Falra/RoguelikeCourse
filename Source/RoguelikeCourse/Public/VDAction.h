// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VDAction.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API UVDAction : public UObject
{
	GENERATED_BODY()
public:

	/* Action nickname to start/stop w/o a reference to thee object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
};
