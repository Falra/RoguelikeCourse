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

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly) // RepNotify
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
};
