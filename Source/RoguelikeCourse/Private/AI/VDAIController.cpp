// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VDAIController.h"

void AVDAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}
