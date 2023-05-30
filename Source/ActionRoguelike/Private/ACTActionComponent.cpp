// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTActionComponent.h"
#include "ACTAction.h"
#include "AudioDevice.h"
#include "Net/UnrealNetwork.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"

void UACTActionComponent::AddAction(AActor* Instigator, TSubclassOf<UACTAction> ActionClass)
{
	if(!ensure(ActionClass)) return;
	UACTAction* Action = NewObject<UACTAction>(GetOwner(),ActionClass);
	if(ensure(Action))
	{
		FString DebugMsg = FString::Printf(TEXT("Add Action %s"),*Action->ActionName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
		Action->Initialize(this);
		Actions.Add(Action);
		if(Action->bAutoStart && ensure(Action->CanStart(Instigator)))
		{
			Action->StartAction(Instigator);
		}
	}
}

bool UACTActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(auto Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString DebugMsg = FString::Printf(TEXT("Failed to run : %s"),*ActionName.ToString());
            	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
				continue;
			}

			// Is Client?
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator,ActionName);
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UACTActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(auto Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void UACTActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator,ActionName);
}

void UACTActionComponent::RemoveAction(UACTAction* ActionToMove)
{
	if(!ensure(ActionToMove && !ActionToMove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToMove);
}

void UACTActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	for(UACTAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action:%s"),*GetNameSafe(GetOwner()),*GetNameSafe(Action));
		LogOnScreen(this,ActionMsg,TextColor,0.0f);
	}
}

bool UACTActionComponent::HasAction(TSubclassOf<UACTAction> ActionClass)
{
	for(UACTAction* Action : Actions)
	{
		if(Action && Action->IsA(ActionClass))
		{
			FString DebugMsg = FString::Printf(TEXT("Already had action : %s"),*Action->ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return true;
		}
	}
	return false;
}

void UACTActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for(TSubclassOf<UACTAction> ActionClass : DefaultActions)
		{
			if(HasAction(ActionClass))
			{
				continue;
			}
			AddAction(GetOwner(), ActionClass);
		}
	}
}

bool UACTActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(UACTAction* Action : Actions)
	{
		if(Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action,*Bunch,*RepFlags);
		}
	}
	
	return WroteSomething;
}

UACTActionComponent::UACTActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UACTActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UACTActionComponent,Actions);
}