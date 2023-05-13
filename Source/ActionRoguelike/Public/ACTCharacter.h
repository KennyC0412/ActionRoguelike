// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACTCharacter.generated.h"

class UACTActionComponent;
class UACTCreditsComponent;
class UACTAttributeComponent;
class UACTInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACTCharacter();

protected:

	UPROPERTY(VisibleAnywhere,Category="Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UACTInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UACTAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UACTCreditsComponent* CreditsComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UACTActionComponent* ActionComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void SprintStart();
	void SprintStop();
	void PrimaryAttack();
	void BlackHoleAttack();
	void DashAttack();
	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth,float Delta);

	virtual void PostInitializeComponents() override;

	UACTAttributeComponent* GetAttrributeComp() { return AttributeComp; }

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	UFUNCTION(Exec)
	void KillSelf();

};
