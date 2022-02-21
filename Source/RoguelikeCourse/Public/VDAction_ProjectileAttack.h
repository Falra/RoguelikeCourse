// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VDAction.h"
#include "VDAction_ProjectileAttack.generated.h"

class UParticleSystem;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class ROGUELIKECOURSE_API UVDAction_ProjectileAttack : public UVDAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimationDelay = 0.2f;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	UVDAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
};
