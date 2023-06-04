// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAnimInstance.h"

#include "ACTActionComponent.h"
#include "GameplayTagContainer.h"

void UACTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AActor* OwningActor = GetOwningActor();
	if(OwningActor)
	{
		ActionComp = Cast<UACTActionComponent>(OwningActor->GetComponentByClass(UACTActionComponent::StaticClass()));
	}
}

void UACTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if(ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
