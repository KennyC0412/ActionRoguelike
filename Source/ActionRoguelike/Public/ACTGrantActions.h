// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTGameplayBase.h"
#include "ACTAction.h"
#include "ACTGrantActions.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTGrantActions : public AACTGameplayBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Action")
	TArray<TSubclassOf<UACTAction>> CanGainActions;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	AACTGrantActions();
	
	void BeginPlay();
};
