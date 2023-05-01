// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTProjectileBase.h"
#include "ACTMagicProjectile.generated.h"

class URadialForceComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTMagicProjectile : public AACTProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACTMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnActorHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	void Explode_Implementation();

public:

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
