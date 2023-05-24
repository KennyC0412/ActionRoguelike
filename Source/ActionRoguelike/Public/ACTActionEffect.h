// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTAction.h"
#include "ACTActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UACTActionEffect : public UACTAction
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent,Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:
	UACTActionEffect();
};
