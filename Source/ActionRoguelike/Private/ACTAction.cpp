// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAction.h"

#include "ACTActionComponent.h"


bool UACTAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	
	UACTActionComponent* Comp = GetOwningComponent();
	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UACTAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"),*GetNameSafe(this));

	UACTActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UACTAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop: %s"),*GetNameSafe(this));

	ensureAlways(bIsRunning);
	UACTActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UACTAction::GetWorld() const
{
	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	if(ActorComponent)
	{
		return ActorComponent->GetWorld();
	}

	return nullptr;
}

UACTActionComponent* UACTAction::GetOwningComponent() const
{
	return Cast<UACTActionComponent>(GetOuter());
}

bool UACTAction::IsRunning() const
{
	return bIsRunning;
}

