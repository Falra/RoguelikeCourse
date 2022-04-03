// Fill out your copyright notice in the Description page of Project Settings.


#include "VDActionEffect.h"
#include "VDActionComponent.h"

UVDActionEffect::UVDActionEffect()
{
	bAutoStart = true;
}

void UVDActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if(Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UVDActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
		
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UVDActionComponent* ActionComponent = GetOwningComponent();
	if(ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
}

float UVDActionEffect::GetTimeRemaining() const
{
	float EndTime = TimeStarted + Duration;
	return EndTime - GetWorld()->TimeSeconds;
}

void UVDActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
