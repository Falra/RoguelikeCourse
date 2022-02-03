// Fill out your copyright notice in the Description page of Project Settings.


#include "VDGameModeBase.h"

#include "DrawDebugHelpers.h"
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

	int32 NumOfAliveBots = 0;
	for(TActorIterator<AVDAICharacter> It(GetWorld()); It; ++It)
	{
		AVDAICharacter* Bot = *It;
		UVDAttributeComponent* AttributeComponent = Cast<UVDAttributeComponent>(Bot->GetComponentByClass(UVDAttributeComponent::StaticClass()));
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumOfAliveBots)
	
	float MaxBotCount = 10.0f;

	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bots capacity. Skipping bot spawn."));
		return;
	}
	
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

	TArray<FVector> Locations;
	bool isResult = QueryInstance->GetQueryResultsAsLocations(Locations);

	if(isResult && Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}
