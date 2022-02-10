// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UVDBTService_CheckAttackRange::UVDBTService_CheckAttackRange()
{
	MaxAttackRange = 2000.f;
}

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
			AAIController* MyController = OwnerComp.GetAIOwner();

			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				float DistanceTo = FVector::Distance(PlayerActor->GetActorLocation(), AIPawn->GetActorLocation());

				bool bWithinRange = DistanceTo < MaxAttackRange;

				bool bHasLOS = false;
				if (bWithinRange)
				{
					bHasLOS = MyController->LineOfSightTo(PlayerActor);
				}

				BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
			}
		}
	}
}

