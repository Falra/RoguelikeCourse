// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDBTService_CheckHealth.h"

#include "AIController.h"
#include "VDAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UVDBTService_CheckHealth::UVDBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}

void UVDBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		UVDAttributeComponent* AttributeComp = UVDAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) < LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}


