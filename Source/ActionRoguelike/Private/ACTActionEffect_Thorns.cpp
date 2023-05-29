// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTActionEffect_Thorns.h"

#include "ACTActionComponent.h"
#include "ACTAttributeComponent.h"
#include "ACTGameplayFunctionLibrary.h"

UACTActionEffect_Thorns::UACTActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;
}

void UACTActionEffect_Thorns::ReflectDamage(AActor* InstigatorActor, UACTAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	if(InstigatorActor == GetOwningComponent()->GetOwner() || Delta >= 0.0f)
	{
		return;
	}
	int32 ActualDamage = -FMath::RoundToInt(Delta * ThornsPercent);
	UACTGameplayFunctionLibrary::ApplyDamage(OwningActor,InstigatorActor,ActualDamage);
}

void UACTActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Display,TEXT("Thorns start"));
	Super::StartAction_Implementation(Instigator);

	UACTAttributeComponent* AttributeComponent = UACTAttributeComponent::GetAttributes(Instigator);
	if(AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this,&UACTActionEffect_Thorns::ReflectDamage);
	}
}

void UACTActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UACTAttributeComponent* AttributeComponent = UACTAttributeComponent::GetAttributes(Instigator);
	if(AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this,&UACTActionEffect_Thorns::ReflectDamage);
	}
}
