// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VDTargetDummy.generated.h"

class UVDAttributeComponent;

UCLASS()
class ROGUELIKECOURSE_API AVDTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVDTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UVDAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth, float DeltaHealth);
	
};
