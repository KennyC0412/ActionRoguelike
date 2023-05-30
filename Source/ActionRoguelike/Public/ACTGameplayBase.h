// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACTGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ACTGameplayBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AACTGameplayBase : public AActor, public IACTGameplayInterface
{
	GENERATED_BODY()
	
public:	
	AACTGameplayBase();
protected:
	
	UPROPERTY(EditDefaultsOnly,ReplicatedUsing="OnRep_Interact",BlueprintReadOnly)
	bool bIsVisible = true;

	UPROPERTY(EditDefaultsOnly)
    float RespawnTime;

	UFUNCTION()
	void OnRep_Interact();
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent*  BaseMesh;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	void ShowUp();

	void HideAndCooldownPowerup();

	FTimerHandle RespawnTimerHandle;

	virtual void ResetVisibility(UStaticMeshComponent* StaticMesh, bool bIsActive);
public:
	
};
