// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACTAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UACTAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UACTAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Attributes")
	static UACTAttributeComponent* GetAttributes(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	// Sets default values for this component's properties
	UACTAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float HealthMax;

public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsFull() const;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	

		
};
