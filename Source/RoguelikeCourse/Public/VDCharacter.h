// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UVDInteractionComponent;
class UVDAttributeComponent;

UCLASS()
class ROGUELIKECOURSE_API AVDCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float TimerAttack_InRate = 0.2f;

	FTimerHandle TimerHandle_PrimaryAttack;

public:
	// Sets default values for this character's properties
	AVDCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UVDInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVDAttributeComponent* AttributeComponent;
	
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
