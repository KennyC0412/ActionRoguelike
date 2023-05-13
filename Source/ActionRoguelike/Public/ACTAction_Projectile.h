// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTAction.h"
#include "ACTAction_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UACTAction_Projectile : public UACTAction
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere)
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere)
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere,Category="Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* CastSpellVFX;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorActor);

public:
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	UACTAction_Projectile();
};
