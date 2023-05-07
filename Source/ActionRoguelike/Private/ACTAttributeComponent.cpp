// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAttributeComponent.h"

// Sets default values for this component's properties
UACTAttributeComponent::UACTAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Health = 100.0f;
}

bool UACTAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UACTAttributeComponent::IsFull() const
{
	return Health >= HealthMax;
}


bool UACTAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr,this,Health,ActualDelta);
	return true;
}


