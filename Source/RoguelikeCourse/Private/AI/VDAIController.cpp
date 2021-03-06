// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AVDAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("Behavior Tree is not assigned. Please assign BehaviorTree in AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if(MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
}
