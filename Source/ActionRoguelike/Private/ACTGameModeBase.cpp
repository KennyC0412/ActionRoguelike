// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameModeBase.h"

#include "ACTActionComponent.h"
#include "ACTAttributeComponent.h"
#include "ACTCharacter.h"
#include "ACTCoin.h"
#include "ACTMonsterData.h"
#include "ACTPlayerState.h"
#include "ACTSaveGame.h"
#include "EngineUtils.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/ACTAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("act.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);


AACTGameModeBase::AACTGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	PlayerStateClass = AACTPlayerState::StaticClass();
	DesiredCount = 25;
	SlotName = "SaveGame01";
}

void AACTGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&AACTGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
	if(ensure(PowerUpClass.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,SpawnPowerUpQuery,this,EEnvQueryRunMode::AllMatching,nullptr);
		if(ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AACTGameModeBase::OnPowerUpQueryCompleted);
		}
	}
}

void AACTGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void AACTGameModeBase::OnMonsterLoaded(FPrimaryAssetId PrimaryAssetId, FVector SpawnLocation)
{
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if(AssetManager)
	{
		UACTMonsterData* MonsterData = Cast<UACTMonsterData>(AssetManager->GetPrimaryAssetObject(PrimaryAssetId));
		if(MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass,SpawnLocation,FRotator::ZeroRotator);
            if(NewBot)
            {
            	LogOnScreen(this, FString::Printf(TEXT("Spawn enemy: %s (%s)"),*GetNameSafe(NewBot),*GetNameSafe(MonsterData)));
        
            	UACTActionComponent* ActionComp = Cast<UACTActionComponent>(NewBot->GetComponentByClass(UACTActionComponent::StaticClass()));
            	if(ActionComp)
            	{
            		for(TSubclassOf<UACTAction> ActionClass : MonsterData->Actions)
            		{
            			ActionComp->AddAction(NewBot, ActionClass);
            		}
            	}
            }
		}
	}
}

void AACTGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Display,TEXT("Spawn query failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	if(Locations.IsValidIndex(0))
	{
		if(MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("",Rows);

			int32 RandomIndex = FMath::RandRange(0,Rows.Num()-1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* AssetManager = UAssetManager::GetIfValid();
			if(AssetManager)
			{
				TArray<FName> Bundles;
				LogOnScreen(this, "Loading Monster...", FColor::Green);

				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AACTGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				AssetManager->LoadPrimaryAsset(SelectedRow->MonsterId,Bundles,Delegate);
			}
		}
	}
}

void AACTGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via cvar 'CVarSpawnBots"));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for(TActorIterator<AACTAICharacter> It(GetWorld());It; ++It)
	{
		AACTAICharacter* Bot = *It;
		
		UACTAttributeComponent* AttributeComp = UACTAttributeComponent::GetAttributes(Bot);
		if(AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	
	float MaxBotCount = 10;
	if(NrOfAliveBots >= MaxBotCount) return;

	if(DifficultCurve)
	{
		MaxBotCount = DifficultCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest25Pct,nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AACTGameModeBase::OnQueryCompleted);
	}
}

void AACTGameModeBase::OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Display,TEXT("Spawn query failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	TArray<FVector> UsedLocation;
	
	int SpawnCount = 0;
	while(SpawnCount < DesiredCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0,Locations.Num()-1);

		FVector SpawnLocation = Locations[RandomLocationIndex];

		Locations.RemoveAt(RandomLocationIndex);
		bool bValidLocation = true;
		for(FVector OtherLocation : UsedLocation)
		{
			//check distance
			float DistanceTo = (SpawnLocation - OtherLocation).Size();
			if(DistanceTo < RequiredDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		//Failed the distance check
		if(!bValidLocation)
		{
			continue;
		}

		//Pick a random powerup class
		int32 RandomClassIndex = FMath::RandRange(0,PowerUpClass.Num()-1);
		GetWorld()->SpawnActor<AActor>(PowerUpClass[RandomClassIndex],SpawnLocation,FRotator::ZeroRotator);

		UsedLocation.Add(SpawnLocation);
		SpawnCount++;
	}
}

void AACTGameModeBase::RespawnPlayerElapsed(APlayerController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void AACTGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AACTPlayerState* PS = NewPlayer->GetPlayerState<AACTPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}


void AACTGameModeBase::KillAllAI()
{
	for(TActorIterator<AACTAICharacter> It(GetWorld());It; ++It)
	{
		AACTAICharacter* Bot = *It;
		UACTAttributeComponent* AIAttributeComp = UACTAttributeComponent::GetAttributes(Bot);
		if(ensure(AIAttributeComp) && AIAttributeComp->IsAlive())
		{
			AIAttributeComp->Kill(Bot);			
		}
	}
}

void AACTGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AACTCharacter* Player = Cast<AACTCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());
		Player->SetLifeSpan(3.0f);
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,TimerDelegate,5.0f,false);
	}
	else
	{
		AACTAICharacter* AIPawn = Cast<AACTAICharacter>(VictimActor);
		if(AIPawn)
		{
			AACTCharacter* MyPlayer = Cast<AACTCharacter>(Killer);
			if(MyPlayer)
			{
				AACTPlayerState* PS = Cast<AACTPlayerState>(MyPlayer->GetPlayerState());
				if(PS)
				{
					PS->AddCredits(CreditsPerKill);
					PS->ApplyCoins(CreditsPerKill);
				}
			}
		}
	}
}

void AACTGameModeBase::WriteSaveGame()
{
	for(int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		AACTPlayerState* PS = Cast<AACTPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	for(FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if(!Actor->Implements<UACTGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		Ar.ArIsSaveGame = true;
		
		Actor->Serialize(Ar);
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void AACTGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame = Cast<UACTSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp,Warning, TEXT("Failed to load savegame data"));
			return;
		}
		UE_LOG(LogTemp,Warning, TEXT("load savegame data"));
		
		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if(!Actor->Implements<UACTGameplayInterface>())
			{
				continue;
			}
			for(FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					IACTGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UACTSaveGame>(UGameplayStatics::CreateSaveGameObject(UACTSaveGame::StaticClass()));
		UE_LOG(LogTemp,Warning, TEXT("create new savegame data"));
	}
}