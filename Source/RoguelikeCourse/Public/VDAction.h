// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VDAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROGUELIKECOURSE_API UVDAction : public UObject
{
	GENERATED_BODY()
public:

	/* Action nickname to start/stop w/o a reference to thee object */
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void StopAction(AActor* Instigator);
};
