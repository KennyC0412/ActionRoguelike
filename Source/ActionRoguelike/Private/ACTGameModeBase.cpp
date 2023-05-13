// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameModeBase.h"

#include "ACTAttributeComponent.h"
#include "ACTCharacter.h"
#include "ACTCoin.h"
#include "ACTCreditsComponent.h"
#include "EngineUtils.h"
#include "AI/ACTAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("act.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);


AACTGameModeBase::AACTGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AACTGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&AACTGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnCoins,this,&AACTGameModeBase::SpawnCoinTimerElapsed,SpawnTimerInterval,true);
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
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);
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

void AACTGameModeBase::OnCoinQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Display,TEXT("Spawn query failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(CoinClass,Locations[0],FRotator::ZeroRotator);
	}
}

void AACTGameModeBase::SpawnCoinTimerElapsed()
{
	int32 NrOfCoins = 0;
	for(TActorIterator<AACTCoin> It(GetWorld()); It; ++It)
	{
		AACTCoin* Coin = *It;
		if(Coin)
		{
			NrOfCoins++;
		}
	}

	float MaxCoinCount = 10;
	if(NrOfCoins >= MaxCoinCount) return;

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,SpawnCoinQuery,this,EEnvQueryRunMode::RandomBest25Pct,nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AACTGameModeBase::OnCoinQueryCompleted);
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
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,TimerDelegate,2.0f,false);
	}
	else
	{
		AACTAICharacter* AIPawn = Cast<AACTAICharacter>(VictimActor);
		if(AIPawn)
		{
			AACTCharacter* MyPlayer = Cast<AACTCharacter>(Killer);
			if(MyPlayer)
			{
				UACTCreditsComponent* CreditsComponent = UACTCreditsComponent::GetCredits(MyPlayer);
				CreditsComponent->ApplyScore(1);
			}
		}
	}
}
