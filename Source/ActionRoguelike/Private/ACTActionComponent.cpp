// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTActionComponent.h"
#include "ACTAction.h"

void UACTActionComponent::AddAction(TSubclassOf<UACTAction> ActionClass)
{
	if(!ensure(ActionClass)) return;
	
	UACTAction* Action = NewObject<UACTAction>(this,ActionClass);
	if(ensure(Action))
	{
		Actions.Add(Action);
	}
}

bool UACTActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(auto Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

UACTActionComponent::UACTActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UACTActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<UACTAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}


void UACTActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

