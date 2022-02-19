// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UVDInteractionComponent;
class UVDAttributeComponent;
class UVDActionComponent;

UCLASS()
class ROGUELIKECOURSE_API AVDCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float TimerAttack_InRate = 0.2f;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;
	
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackholeAttack;
	FTimerHandle TimerHandle_Dash;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UVDInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVDAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVDActionComponent* ActionComponent;
	
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void BlackHoleAttack();

	void BlackholeAttack_TimeElapsed();

	void Dash();

	void StartAttackEffects();
	
	void Dash_TimeElapsed();

	// Re-use spawn logic between attacks
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	
	void PrimaryInteract();
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth, float DeltaHealth);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;
	
public:

	// Sets default values for this character's properties
	AVDCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
