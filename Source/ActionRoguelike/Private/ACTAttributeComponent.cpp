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



bool UACTAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr,this,Health,Delta);
	return true;
}


