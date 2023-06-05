// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "ACTGameModeBase.generated.h"

class UDataTable;
class UACTSaveGame;
class UACTMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 5.0f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UACTMonsterData* MonsterData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AACTGameModeBase();
protected:

	FString SlotName;

	UPROPERTY()
	UACTSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UDataTable* MonsterTable;
	
	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_SpawnCoins;

	UPROPERTY(EditDefaultsOnly,Category="Credits")
	int32 CreditsPerKill = 5;
	
	UPROPERTY(EditDefaultsOnly,Category="Credits")
	int32 DesiredCount = 25;

	UPROPERTY(EditDefaultsOnly,Category="Credits")
	float RequiredDistance = 1000;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnPowerUpQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* DifficultCurve;
	
	/*UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> MinionClass;*/

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TArray<TSubclassOf<AActor>> PowerUpClass;
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	virtual void StartPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintCallable)
	void RespawnPlayerElapsed(APlayerController* Controller);

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAllAI();

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

};
