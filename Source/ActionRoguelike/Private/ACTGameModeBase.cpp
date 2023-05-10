// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameModeBase.h"

#include "ACTAttributeComponent.h"
#include "EngineUtils.h"
#include "AI/ACTAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AACTGameModeBase::AACTGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AACTGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&AACTGameModeBase::SpawnBotTimerElaspsed,SpawnTimerInterval,true);
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

void AACTGameModeBase::SpawnBotTimerElaspsed()
{
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
