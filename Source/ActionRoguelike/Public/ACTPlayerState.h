// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ACTPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCoinsChanged,AACTPlayerState*, PlayerState,int32 , NewCredits, int32, Delta);
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,Category="Credits");
	int32 Credits;

	UPROPERTY(EditDefaultsOnly,Category="Credits");
	int32 Coins;

public:
	AACTPlayerState();
	
	UFUNCTION(BlueprintCallable, Category= "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category= "Credits")
	int32 GetCoins() const;

	UFUNCTION(BlueprintCallable, Category= "Credits")
    void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category= "Credits")
	void AddCoins(int32 Delta);

	UFUNCTION(BlueprintCallable, Category= "Credits")
	bool RemoveCoins(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category= "Credits")
	FOnCoinsChanged OnCoinsChanged;
};
