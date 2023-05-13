// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameplayFunctionLibrary.h"

#include "ACTAttributeComponent.h"

bool UACTGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UACTAttributeComponent* AttributeComp = UACTAttributeComponent::GetAttributes(TargetActor);
	if(AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser,-DamageAmount);
	}
	return false;
}

bool UACTGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if(ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.0f,HitResult.ImpactPoint,HitResult.BoneName);
		}
		return true;
	}
	return false;
}
