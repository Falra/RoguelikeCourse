// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UVDBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check distance to player
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComponent))
	{
		AActor* PlayerActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if(PlayerActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if(ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if(ensure(AIPawn))
				{
					float DistanceToPlayer = FVector::Distance(PlayerActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinAttackRange = DistanceToPlayer < 2000.0f;
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinAttackRange);
				}
			}
		}
	}
}
