// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTActionEffect.h"

#include "ACTActionComponent.h"
#include "GameFramework/GameStateBase.h"

void UACTActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

void UACTActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"StopAction",Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle,Delegate,Duration,false);
	}

	if(Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"ExecutePeriodicEffect",Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle,Delegate,Period,true);
	}
}

void UACTActionEffect::StopAction_Implementation(AActor* Instigator)
{
	//avoid miss last execute 
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UACTActionComponent* Comp = GetOwningComponent();
	if(Comp)
	{
		Comp->RemoveAction(this);
	}
}

float UACTActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if(GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	return Duration;
}

UACTActionEffect::UACTActionEffect()
{
	bAutoStart = true;
}
