// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ACTMonsterData.generated.h"

class UACTAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UACTMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TArray<TSubclassOf<UACTAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	UTexture2D* Icon;
};
