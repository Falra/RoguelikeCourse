// Fill out your copyright notice in the Description page of Project Settings.


#include "VDGameModeBase.h"

#include "EngineUtils.h"
#include "VDAttributeComponent.h"
#include "AI/VDAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AVDGameModeBase::AVDGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AVDGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AVDGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AVDGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryWrapper = UEnvQueryManager::RunEQSQuery(this,
		SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if(ensure(QueryWrapper))
	{
		QueryWrapper->GetOnQueryFinishedEvent().AddDynamic(this, &AVDGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void AVDGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		return;
	}

	int32 NumOfAliveBots = 0;
	for(TActorIterator<AVDAICharacter> It(GetWorld()); It; ++It)
	{
		AVDAICharacter* Bot = *It;
		UVDAttributeComponent* AttributeComponent = Cast<UVDAttributeComponent>(Bot->GetComponentByClass(UVDAttributeComponent::StaticClass()));
		if(AttributeComponent && AttributeComponent->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NumOfAliveBots >= MaxBotCount)
	{
		return;
	}
	
	TArray<FVector> Locations;
	bool isResult = QueryInstance->GetQueryResultsAsLocations(Locations);

	if(isResult && Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
