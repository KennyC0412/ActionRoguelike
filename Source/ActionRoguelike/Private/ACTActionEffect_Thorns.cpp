// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTActionEffect_Thorns.h"

#include "ACTActionComponent.h"
#include "ACTAttributeComponent.h"

UACTActionEffect_Thorns::UACTActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;
}

void UACTActionEffect_Thorns::ReflectDamage(AActor* OtherActor, UACTAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(OtherActor == GetOwningComponent()->GetOwner() || Delta > 0)
	{
		return;
	}
	int32 ActualDamage = Delta * ThornsPercent * 1000;
	UACTAttributeComponent* OtherComp = UACTAttributeComponent::GetAttributes(OtherActor);
	OtherComp->ApplyHealthChange(GetOwningComponent()->GetOwner(), ActualDamage);
}

void UACTActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Display,TEXT("Thorns start"));
	Super::StartAction_Implementation(Instigator);

	FTimerDelegate Delegate;
	UACTAttributeComponent* AttributeComponent = UACTAttributeComponent::GetAttributes(Instigator);
	AttributeComponent->OnHealthChanged.AddDynamic(this,&UACTActionEffect_Thorns::ReflectDamage);
	//Instigator->GetWorldTimerManager().SetTimer();
}

void UACTActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	UACTAttributeComponent* AttributeComponent = UACTAttributeComponent::GetAttributes(Instigator);
	AttributeComponent->OnHealthChanged.RemoveDynamic(this,&UACTActionEffect_Thorns::ReflectDamage);
}
