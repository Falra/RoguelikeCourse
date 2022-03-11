// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKECOURSE_API UVDInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	void PrimaryInteract();

protected:

	void FindBestInteractable();
	
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UVDInteractionComponent();
		
};
