// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTActionEffect.h"
#include "ACTActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UACTActionEffect_Thorns : public UACTActionEffect
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	float ThornsPercent;

	UFUNCTION()
	void ReflectDamage(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth, float Delta);

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

    virtual void StopAction_Implementation(AActor* Instigator) override;

	UACTActionEffect_Thorns();
};
