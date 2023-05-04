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

	void ResetVisibility();
	// Sets default values for this actor's properties
	AACTHealthPotion();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle VisibilityTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
