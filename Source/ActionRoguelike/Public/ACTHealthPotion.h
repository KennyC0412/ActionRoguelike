// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTGameplayBase.h"
#include "GameFramework/Actor.h"
#include "ACTGameplayInterface.h"
#include "ACTHealthPotion.generated.h"

class AACTCharacter;

UCLASS()
class ACTIONROGUELIKE_API AACTHealthPotion : public AACTGameplayBase
{
	GENERATED_BODY()
	
public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	// Sets default values for this actor's properties
	AACTHealthPotion();

protected:

	UFUNCTION(Server,Reliable)
	void ServerInteract(APawn* InstigatorPawn);
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	float Yaw = 0.0f;

	int32 Cost;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

};
