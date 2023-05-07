// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACTAICharacter.generated.h"

class UACTAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AACTAICharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere)
	UACTAttributeComponent* AttributeComp;
		
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
public:	

};
