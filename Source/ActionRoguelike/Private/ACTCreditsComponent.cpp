// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTCreditsComponent.h"

UACTCreditsComponent* UACTCreditsComponent::GetCredits(AActor* FromActor)
{
	if(!FromActor) return nullptr;
	
	return Cast<UACTCreditsComponent>(FromActor->GetComponentByClass(UACTCreditsComponent::StaticClass()));
}

// Sets default values for this component's properties
UACTCreditsComponent::UACTCreditsComponent()
{
	Credits = 0;
	Coins = 0;
}

// Called when the game starts
void UACTCreditsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UACTCreditsComponent::IsEnoughCost(int cost)
{
	return Coins >= cost;
}

void UACTCreditsComponent::ApplyScore(int credit)
{
	Credits = FMath::Clamp(Credits + credit,0,INT_MAX);
}

void UACTCreditsComponent::ApplyCoin(int coin)
{
	Coins = FMath::Clamp(Coins + coin,0,INT_MAX);
}
