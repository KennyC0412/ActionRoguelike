// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAttributeComponent.h"
#include "Net/UnrealNetwork.h"
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
	RageMax = 100.0f;
	Rage = 0.0f;
	
	SetIsReplicatedByDefault(true);
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

float UACTAttributeComponent::GetRage() const
{
	return Rage;
}

void UACTAttributeComponent::ReduceRage(float RageToReduce)
{
	Rage = FMath::Clamp(Rage - RageToReduce,0.0f,RageMax);
}

bool UACTAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	
	if(Delta <= 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
		ApplyRage(InstigatorActor,-Delta);
	}
	
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = Health - OldHealth;

	if(ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor,Health,ActualDelta);
	}
	if(ActualDelta < 0.0f && Health == 0.0f)
	{
		AACTGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AACTGameModeBase>();
		if(GameMode)
		{
			GameMode->OnActorKilled(GetOwner(),InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

bool UACTAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	Rage = FMath::Clamp(Rage + Delta,0.0f,RageMax);
	if(Delta != 0.0f)
	{
		MulticastRageChanged_Implementation(InstigatorActor,Rage,Delta);
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

void UACTAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UACTAttributeComponent,Health);
	DOREPLIFETIME(UACTAttributeComponent,HealthMax);
	DOREPLIFETIME(UACTAttributeComponent,Rage);
	DOREPLIFETIME(UACTAttributeComponent,RageMax);
	//DOREPLIFETIME(UACTAttributeComponent,HealthMax,COND_OwnerOnly);

}

void UACTAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,Delta);
}

void UACTAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor,this,Rage,Delta);
}