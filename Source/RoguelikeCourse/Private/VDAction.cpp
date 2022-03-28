// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAction.h"

#include "VDActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "RoguelikeCourse/RoguelikeCourse.h"

void UVDAction::Initialize(UVDActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;	
}

void UVDAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UVDActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UVDAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	//ensureAlways(bIsRunning);
	
	UVDActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

UVDActionComponent* UVDAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(UVDActionComponent::StaticClass());
	//return Cast<UVDActionComponent>(GetOuter());
	return ActionComp;
}

void UVDAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UVDAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool UVDAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	
	UVDActionComponent* ActionComponent = GetOwningComponent();
	if(ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

UWorld* UVDAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

void UVDAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVDAction, RepData);
	DOREPLIFETIME(UVDAction, ActionComp);
}
