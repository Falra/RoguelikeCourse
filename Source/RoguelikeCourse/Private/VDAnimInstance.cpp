// Fill out your copyright notice in the Description page of Project Settings.


#include "VDAnimInstance.h"

#include "GameplayTagContainer.h"
#include "VDActionComponent.h"

void UVDAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if(OwningActor)
	{
		ActionComp = OwningActor->FindComponentByClass<UVDActionComponent>();
	}
}

void UVDAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if(ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
