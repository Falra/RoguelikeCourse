// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VDInteractionComponent.generated.h"

class UVDWorldUserWidget;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UVDWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UVDWorldUserWidget* DefaultWidgetInstance;
	
public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UVDInteractionComponent();
		
};
