// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACTCharacter.generated.h"

class UACTAttributeComponent;
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
	TSubclassOf<AActor> NormalProjectileClass;

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> DashProjectileClass;

	FTimerHandle TimerHandle_PrimaryAttack;

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
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void DashAttack_TimeElapsed();
	void NormalAttack_TimeElapsed();
	void MagicAttack_TimeElapsed();
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	void NormalAttack();
	void MagicAttack();
	void DashAttack();
	void PrimaryInteract();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth,float Delta);

	virtual void PostInitializeComponents() override;
};
