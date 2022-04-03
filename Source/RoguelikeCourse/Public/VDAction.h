// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "VDAction.generated.h"


class UVDActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ROGUELIKECOURSE_API UVDAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Replicated)
	UVDActionComponent* ActionComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UVDActionComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(Replicated)
	float TimeStarted;
	
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

	UFUNCTION()
	void OnRep_RepData();
	
public:

	void Initialize(UVDActionComponent* NewActionComp);
	
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

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
