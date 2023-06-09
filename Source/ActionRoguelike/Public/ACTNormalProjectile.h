// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTProjectileBase.h"
#include "GameplayTagContainer.h"
#include "ACTNormalProjectile.generated.h"

class UACTActionEffect;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTNormalProjectile : public AACTProjectileBase
{
	GENERATED_BODY()

public:
	AACTNormalProjectile();
protected:

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<UACTActionEffect> EffectClass;
	
	virtual void BeginPlay() override;
	
	virtual void OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
