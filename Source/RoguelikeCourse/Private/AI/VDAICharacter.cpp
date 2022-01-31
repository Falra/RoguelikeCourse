// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDAICharacter.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AVDAICharacter::AVDAICharacter()
{
 	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AVDAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AVDAICharacter::OnPawnSeen);
}

void AVDAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		BlackboardComponent->SetValueAsObject("TargetActor", Pawn);
		// DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
	
}
