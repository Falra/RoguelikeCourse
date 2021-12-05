// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "VDItemChest.generated.h"

UCLASS()
class ROGUELIKECOURSE_API AVDItemChest : public AActor, public IVDGameplayInterface
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	// Sets default values for this actor's properties
	AVDItemChest();

	void Interact_Implementation(APawn* InstigatorPawn);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
