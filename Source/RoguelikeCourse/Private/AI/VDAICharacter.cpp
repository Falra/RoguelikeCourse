// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "VDAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AVDAICharacter::AVDAICharacter()
{
 	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComponent = CreateDefaultSubobject<UVDAttributeComponent>("AttributeComponent");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AVDAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AVDAICharacter::OnPawnSeen);

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AVDAICharacter::OnHealthChanged);
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

void AVDAICharacter::OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth,
	float DeltaHealth)
{
	if(DeltaHealth < 0.0f)
	{

		if(NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// rag-doll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
