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
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent*  BaseMesh;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void ShowUp_Implementation() override;

	virtual void Hide_Implementation() override;
	
	virtual void ResetVisibility(bool bIsActive);
public:
	
};
