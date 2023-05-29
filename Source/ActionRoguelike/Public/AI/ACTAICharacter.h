// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACTAICharacter.generated.h"

class UACTWorldUserWidget;
class UACTAttributeComponent;
class UPawnSensingComponent;
class UACTActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AACTAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AACTAICharacter();

protected:

	void SetTargetActor(AActor* NewTarget);
	AActor* GetTargetActor() const;

	UPROPERTY(VisibleAnywhere)
	UACTWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UACTWorldUserWidget* SpottedWidget;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere)
	UACTAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UACTActionComponent* ActionComp;
	
	UPROPERTY(VisibleAnywhere)
	FName TimeToHitParamName;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	FTimerHandle SpottedHidenHandle;

	UPROPERTY(VisibleAnywhere)
	FName TargeActorKey;
public:	

};
