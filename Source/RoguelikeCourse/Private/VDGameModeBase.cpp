// Fill out your copyright notice in the Description page of Project Settings.


#include "VDGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "VDAttributeComponent.h"
#include "VDCharacter.h"
#include "AI/VDAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("vd.SpawnBots"), true, TEXT("Enabling spawning of bots via timer"), ECVF_Cheat);

AVDGameModeBase::AVDGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AVDGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AVDGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AVDGameModeBase::KillAll()
{
	for(TActorIterator<AVDAICharacter> It(GetWorld()); It; ++It)
	{
		AVDAICharacter* Bot = *It;
		UVDAttributeComponent* AttributeComponent = UVDAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); // @fixme: pass in player? for credits	
		}
	}	
}

void AVDGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bots disabled via cvar 'CVarSpawnBots'"));
		return;
	}
	
	int32 NumOfAliveBots = 0;
	for(TActorIterator<AVDAICharacter> It(GetWorld()); It; ++It)
	{
		AVDAICharacter* Bot = *It;
		UVDAttributeComponent* AttributeComponent = UVDAttributeComponent::GetAttributes(Bot);
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

void AVDGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AVDCharacter* Player = Cast<AVDCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		float RespawnDelay = 2.0f;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false); 
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void AVDGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
