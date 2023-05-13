// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAction.h"

void UACTAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"),*GetNameSafe(this));
}

void UACTAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop: %s"),*GetNameSafe(this));
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
