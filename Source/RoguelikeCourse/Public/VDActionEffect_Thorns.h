// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDActionEffect.h"
#include "VDActionEffect_Thorns.generated.h"

class UVDAttributeComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API UVDActionEffect_Thorns : public UVDActionEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	UVDActionEffect_Thorns();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;
	
};
