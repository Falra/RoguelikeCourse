// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAction.h"

#include "VDActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "RoguelikeCourse/RoguelikeCourse.h"

void UVDAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UVDActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UVDAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	ensureAlways(bIsRunning);
	
	UVDActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UVDActionComponent* UVDAction::GetOwningComponent() const
{
	return Cast<UVDActionComponent>(GetOuter());
}

void UVDAction::OnRep_IsRunning()
{
	if(bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

bool UVDAction::IsRunning() const
{
	return bIsRunning;
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
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

void UVDAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVDAction, bIsRunning);
}
