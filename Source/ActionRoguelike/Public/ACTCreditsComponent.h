// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACTCreditsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UACTCreditsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Credits")
    static UACTCreditsComponent* GetCredits(AActor* FromActor);
	
	// Sets default values for this component's properties
	UACTCreditsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Credits");
	int32 Credits;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Credits");
    int32 Coins;

public:	

	UFUNCTION(BlueprintCallable)
	bool IsEnoughCost(int cost);
	
	void ApplyScore(int credit);
	void ApplyCoin(int coin);
};
