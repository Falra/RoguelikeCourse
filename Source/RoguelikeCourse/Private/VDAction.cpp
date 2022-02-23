// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAction.h"

#include "VDActionComponent.h"

void UVDAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UVDActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UVDAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	UVDActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
}

UVDActionComponent* UVDAction::GetOwningComponent() const
{
	return Cast<UVDActionComponent>(GetOuter());
}

bool UVDAction::CanStart_Implementation(AActor* Instigator)
{
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
