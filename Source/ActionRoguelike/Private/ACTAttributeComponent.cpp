// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAttributeComponent.h"

#include "ACTGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("act.DamageMultiplier"),1.0f,TEXT("Global damage modifier for attribute component"),ECVF_Cheat);


// Sets default values for this component's properties
UACTAttributeComponent::UACTAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Health = 100.0f;
	HealthMax = 100.0f;
	LowHealth = 30.0f;
}

bool UACTAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-HealthMax);
}

bool UACTAttributeComponent::IsLowHealth() const
{
	return Health <= LowHealth;
}

bool UACTAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UACTAttributeComponent::IsFull() const
{
	return Health >= HealthMax;
}


bool UACTAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	if(Delta <= 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);

	if(ActualDelta < 0.0f && Health == 0.0f)
	{
		AACTGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AACTGameModeBase>();
		if(GameMode)
		{
			GameMode->OnActorKilled(GetOwner(),InstigatorActor);
		}
	}
	return true;
}

UACTAttributeComponent* UACTAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(!FromActor) return nullptr;
	
	return Cast<UACTAttributeComponent>(FromActor->GetComponentByClass(UACTAttributeComponent::StaticClass()));
}

bool UACTAttributeComponent::IsActorAlive(AActor* Actor)
{
	auto AttributeComp = GetAttributes(Actor);
	if(AttributeComp)
		return AttributeComp->IsAlive();
	
	return false;
}
