// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTAttributeComponent.h"
#include "ACTTargetDummy.generated.h"


class UACTAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACTTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	UACTAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth,float Delta);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
