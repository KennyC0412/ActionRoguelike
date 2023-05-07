// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTGameplayInterface.h"
#include "ACTHealthPotion.generated.h"

class AACTCharacter;

UCLASS()
class ACTIONROGUELIKE_API AACTHealthPotion : public AActor, public IACTGameplayInterface
{
	GENERATED_BODY()
	
public:	

	void Interact_Implementation(APawn* InstigatorPawn);
	void ShowUp();

	void ResetVisibility(bool bIsActive);
	// Sets default values for this actor's properties
	AACTHealthPotion();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle VisibilityTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime;

	float Yaw = 0.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
