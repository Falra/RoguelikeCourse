// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDBTTask_RangedAttack.h"

#include "AIController.h"
#include "VDAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UVDBTTask_RangedAttack::UVDBTTask_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

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

		AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(PlayerActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		bool bIsTargetAlive = UVDAttributeComponent::IsActorAlive(PlayerActor);
		if(!bIsTargetAlive)
		{
			return EBTNodeResult::Failed;	
		}

		// Ignore negative pitch to not hit the floor in front itself
		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		FVector Direction = PlayerActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AIPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
