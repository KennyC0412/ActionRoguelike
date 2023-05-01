// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTProjectileActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACTProjectileActor();

protected:
	UPROPERTY(VisibleAnyWhere)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnyWhere)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnyWhere)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnyWhere)
	UParticleSystemComponent* ExplosionComp;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Destroy();

public:

	FTimerHandle DestroyTimerHandle;
	
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
