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
	UE_LOG(LogTemp,Log,TEXT("Stared: %s"),*ActionName.ToString());
	
	//LogOnScreen(this,FString::Printf(TEXT("Started: %s"),*ActionName.ToString()));

	UACTActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UACTAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped: %s"),*ActionName.ToString());
	//LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"),*ActionName.ToString()));
	
	//ensureAlways(bIsRunning);
	
	UACTActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
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

void UACTAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UACTAction::Initialize(UACTActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UACTAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UACTAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UACTAction,RepData);
	DOREPLIFETIME(UACTAction,ActionComp);
}