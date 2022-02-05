// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UVDBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(AIController))
	{
		ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
		if(AIPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(PlayerActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = PlayerActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AIPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
