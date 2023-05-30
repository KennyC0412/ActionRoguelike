// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditDefaultsOnly,Replicated,Category="Credits");
	int32 Credits;

	UPROPERTY(EditDefaultsOnly,Replicated,Category="Credits");
	int32 Coins;

	UFUNCTION(NetMulticast,Reliable)
	void MultiCastCoinChanged(int32 OldState);

	UFUNCTION(NetMulticast,Reliable)
	void MultiCastCreditChanged(int32 OldState);

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

	UPROPERTY(BlueprintAssignable, Category= "Credits")
	FOnCreditsChanged OnCreditsChanged;

	UPROPERTY(BlueprintAssignable, Category= "Credits")
	FOnCoinsChanged OnCoinsChanged;
};
