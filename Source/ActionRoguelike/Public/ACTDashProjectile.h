// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTProjectileBase.h"
#include "ACTDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AACTDashProjectile : public AACTProjectileBase
{
	GENERATED_BODY()

public:
	AACTDashProjectile();

protected:

	UPROPERTY(EditDefaultsOnly)
	float Teleport_Delay = 0.2f;
	
	UPROPERTY(EditDefaultsOnly)
	float Explode_Delay = 0.2f;

	void Explode_Implementation();
	
	virtual void OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;

	virtual void BeginPlay() override;
	void TelePort();
	FTimerHandle TimerHandle;
};
