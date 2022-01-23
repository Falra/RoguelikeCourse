// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VDAICharacter.generated.h"

class  UPawnSensingComponent;

UCLASS()
class ROGUELIKECOURSE_API AVDAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVDAICharacter();

	
protected:
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
