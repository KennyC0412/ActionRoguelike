// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACTCharacter.generated.h"

class UACTInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere,Category="Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	FTimerHandle TimerHandle_PrimaryAttack;

public:
	// Sets default values for this character's properties
	AACTCharacter();

protected:

	UPROPERTY(VisibleAnywhere,Category="View")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere,Category="View")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,Category="View")
	UACTInteractionComponent* InteractionComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack_TimeElapsed();
	void PrimaryAttack();
	void PrimaryInteract();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
