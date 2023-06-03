// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTGameModeBase.h"
#include "ACTSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "ACTPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged,AACTPlayerState*, PlayerState,int32 , NewCredits, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCoinsChanged,AACTPlayerState*, PlayerState,int32 , NewCoins, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,ReplicatedUsing="OnRep_Credits",Category="Credits");
	int32 Credits;

	UPROPERTY(EditDefaultsOnly,ReplicatedUsing="OnRep_Coins",Category="Credits");
	int32 Coins;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

	UFUNCTION()
	void OnRep_Coins(int32 OldCoins);

public:
	AACTPlayerState();

	UFUNCTION(BlueprintCallable, Category= "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category= "Credits")
	int32 GetCoins() const;

	UFUNCTION(BlueprintCallable, Category= "Credits")
    void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category= "Credits")
	bool ApplyCoins(int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UACTSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
    void LoadPlayerState(UACTSaveGame* SaveObject);

	UPROPERTY(BlueprintAssignable, Category= "Credits")
	FOnCreditsChanged OnCreditsChanged;

	UPROPERTY(BlueprintAssignable, Category= "Credits")
	FOnCoinsChanged OnCoinsChanged;
};
