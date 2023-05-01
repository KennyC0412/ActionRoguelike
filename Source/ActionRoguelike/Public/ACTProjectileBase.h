// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AACTProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACTProjectileBase();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* ImpactVFX;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UParticleSystemComponent* EffectComp;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();
	
	void Destroy();

	FTimerHandle DestroyTimerHandle;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};