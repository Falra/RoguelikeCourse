// Fill out your copyright notice in the Description page of Project Settings.


#include "VDGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "VDAttributeComponent.h"
#include "VDCharacter.h"
#include "VDGameplayInterface.h"
#include "VDMonsterData.h"
#include "VDPlayerState.h"
#include "VDSaveGame.h"
#include "AI/VDAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("vd.SpawnBots"), false,
                                                TEXT("Enabling spawning of bots via timer"), ECVF_Cheat);

AVDGameModeBase::AVDGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = AVDPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void AVDGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void AVDGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AVDPlayerState* PS = NewPlayer->GetPlayerState<AVDPlayerState>();
	if(ensure(PS))
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

}

void AVDGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AVDGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	// Make sure we have assigned at least one power-up class
	if (ensure(PowerupClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AVDGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
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
		if(MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// Get random enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];
			GetWorld()->SpawnActor<AActor>(SelectedRow->MonsterData->MonsterClass, Locations[0], FRotator::ZeroRotator);
		}
		else
		{
			GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		}

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void AVDGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                                   EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerup EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void AVDGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	// Respawn Players after delay
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

	// Give Credits for kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	// Don't credit kills of self
	if (KillerPawn && KillerPawn != VictimActor)
	{
		// Only Players will have a 'PlayerState' instance, bots have nullptr
		AVDPlayerState* PS = KillerPawn->GetPlayerState<AVDPlayerState>();
		if (PS) 
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

void AVDGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AVDGameModeBase::WriteSaveGame()
{
	// Iterate all player state
	for (int32 i =0; i < GameState->PlayerArray.Num(); i++)
	{
		AVDPlayerState* PS = Cast<AVDPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single player realization
		}
	}

	CurrentSaveGame->SaveActors.Empty();
	
	// Iterate the entire world of actors
	for(FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = * It;
		if(!Actor->Implements<UVDGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorSaveData;
		ActorSaveData.ActorName = Actor->GetName();
		ActorSaveData.ActorTransform = Actor->GetTransform();

		FMemoryWriter MemoryWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Archive.ArIsSaveGame = true;
		
		Actor->Serialize(Archive);

		CurrentSaveGame->SaveActors.Add(ActorSaveData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AVDGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UVDSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		// Iterate the entire world of actors
		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = * It;
			if(!Actor->Implements<UVDGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorSaveData : CurrentSaveGame->SaveActors)
			{
				if(Actor->GetName() == ActorSaveData.ActorName)
				{
					//UE_LOG(LogTemp, Log, TEXT("Load transform for %s"), *ActorSaveData.ActorName);
					Actor->SetActorTransform(ActorSaveData.ActorTransform);

					FMemoryReader MemoryReader(ActorSaveData.ByteData);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);

					IVDGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UVDSaveGame>(UGameplayStatics::CreateSaveGameObject(UVDSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
