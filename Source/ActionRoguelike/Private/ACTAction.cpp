// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAction.h"

#include "ACTActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"


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
	LogOnScreen(this,FString::Printf(TEXT("Started: %s"),*ActionName.ToString()));

	UACTActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UACTAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"),*ActionName.ToString()));
	
	//ensureAlways(bIsRunning);
	
	UACTActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UACTAction::GetWorld() const
{
	AActor* ActorComponent = Cast<AActor>(GetOuter());
	if(ActorComponent)
	{
		return ActorComponent->GetWorld();
	}

	return nullptr;
}

UACTActionComponent* UACTAction::GetOwningComponent() const
{
	return ActionComp;
}

void UACTAction::OnRep_IsRunning()
{
	if(bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void UACTAction::Initialize(UACTActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UACTAction::IsRunning() const
{
	return bIsRunning;
}

void UACTAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UACTAction,bIsRunning);
	DOREPLIFETIME(UACTAction,ActionComp);
}