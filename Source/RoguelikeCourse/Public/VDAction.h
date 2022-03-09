// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "VDAction.generated.h"


class UVDActionComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class ROGUELIKECOURSE_API UVDAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UVDActionComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:

	/* start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanStart(AActor* Instigator);
	
	/* Action nickname to start/stop w/o a reference to thee object */
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;
};
