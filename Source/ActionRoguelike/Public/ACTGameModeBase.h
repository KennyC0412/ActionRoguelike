// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "ACTGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AACTGameModeBase();
protected:

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* DifficultCurve;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> MinionClass;
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	virtual void StartPlay() override;
	
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBotTimerElapsed();

	UFUNCTION(BlueprintCallable)
	void RespawnPlayerElapsed(APlayerController* Controller);
	
	UFUNCTION(Exec)
	void KillAllAI();
};
