// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameModeBase.h"

#include "ACTAttributeComponent.h"
#include "ACTCharacter.h"
#include "ACTCoin.h"
#include "ACTPlayerState.h"
#include "EngineUtils.h"
#include "AI/ACTAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("act.SpawnBots"),true,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);


AACTGameModeBase::AACTGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	PlayerStateClass = AACTPlayerState::StaticClass();
	DesiredCount = 25;
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
					PS->AddCredits(5);
					PS->AddCoins(5);
				}
			}
		}
	}
}
