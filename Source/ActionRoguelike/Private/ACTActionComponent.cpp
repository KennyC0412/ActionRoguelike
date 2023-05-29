// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTActionComponent.h"
#include "ACTAction.h"
#include "AudioDevice.h"

void UACTActionComponent::AddAction(AActor* Instigator, TSubclassOf<UACTAction> ActionClass)
{
	if(!ensure(ActionClass)) return;
	UACTAction* Action = NewObject<UACTAction>(this,ActionClass);

	if(ensure(Action))
	{
		FString DebugMsg = FString::Printf(TEXT("Add Action %s"),*Action->ActionName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
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

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
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

	for(TSubclassOf<UACTAction> ActionClass : DefaultActions)
	{
		if(HasAction(ActionClass))
		{
			continue;
		}
		AddAction(GetOwner(), ActionClass);
	}
}

UACTActionComponent::UACTActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}