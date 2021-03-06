// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "VDProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VDMagicProjectile.generated.h"

class UVDActionEffect;

UCLASS()
class ROGUELIKECOURSE_API AVDMagicProjectile : public AVDProjectileBase // Re-parented from AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UVDActionEffect> ActionEffectClass;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:	

	AVDMagicProjectile();

};
