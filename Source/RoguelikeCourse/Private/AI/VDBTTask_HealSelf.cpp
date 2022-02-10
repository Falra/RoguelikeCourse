// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDBTTask_HealSelf.h"

#include "AIController.h"
#include "VDAttributeComponent.h"

EBTNodeResult::Type UVDBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UVDAttributeComponent* AttributeComp = UVDAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;
}
