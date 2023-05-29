// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTGameplayBase.h"
#include "ACTGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ACTCoin.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AACTCoin : public AACTGameplayBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACTCoin();

protected:

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(Server,Reliable)
	void ServerInteract(APawn* InstigatorPawn);

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
