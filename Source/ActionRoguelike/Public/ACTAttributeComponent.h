// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACTAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UACTAttributeComponent*, OwningComp, float, NewValue, float, Delta);

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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float LowHealth;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float RageMax;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float Rage;

	UFUNCTION(NetMulticast,Reliable) // @FIXME: mark as unreliable once we moved  the 'state' our of scharacter
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast,Unreliable)
    void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);
public:
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsLowHealth() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsFull() const;

	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	UFUNCTION(BlueprintCallable)
	void ReduceRage(float RageToReduce);
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
    bool ApplyRage(AActor* InstigatorActor, float Delta);

};
