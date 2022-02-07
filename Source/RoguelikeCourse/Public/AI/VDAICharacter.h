// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VDAICharacter.generated.h"

class UVDAttributeComponent;
class  UPawnSensingComponent;
class UVDWorldUserWidget;

UCLASS()
class ROGUELIKECOURSE_API AVDAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVDAICharacter();

	
protected:
	
	void SetTargetActor(AActor* NewTarget);
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVDAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UVDWorldUserWidget* ActiveHealthBar;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth, float DeltaHealth);

};
