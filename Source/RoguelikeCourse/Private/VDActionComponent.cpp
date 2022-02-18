// Fill out your copyright notice in the Description page of Project Settings.


#include "VDActionComponent.h"

#include "VDAction.h"

UVDActionComponent::UVDActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVDActionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UVDActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVDActionComponent::AddAction(TSubclassOf<UVDAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UVDAction* NewAction = NewObject<UVDAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UVDActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	
}

bool UVDActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	
}
