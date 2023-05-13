// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTGameplayBase.h"
#include "GameFramework/Actor.h"
#include "ACTGameplayInterface.h"
#include "ACTItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AACTItemChest : public AACTGameplayBase
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	// Sets default values for this actor's properties
	AACTItemChest();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* GoldMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComp;
	
	UPROPERTY(EditDefaultsOnly)
	bool IsClosed = true;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
