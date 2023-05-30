// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTPlayerState.h"
#include "Net/UnrealNetwork.h"

void AACTPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACTPlayerState,Credits);
	DOREPLIFETIME(AACTPlayerState,Coins);
}


AACTPlayerState::AACTPlayerState()
{
	Coins = 0;
	Credits = 0;
	SetReplicates(true);
}

void AACTPlayerState::AddCredits(int32 Delta)
{
	if(!ensure(Delta > 0.0f))
	{
		return;
	}
	int OldCredits = Credits;
	
	Credits += Delta;
	
	MultiCastCreditChanged_Implementation(OldCredits);
	//OnCreditsChanged.Broadcast(this,Credits,Delta);
}

bool AACTPlayerState::ApplyCoins(int32 Delta)
{
	if(Delta == 0.0f) return true;
	
	if(Delta < 0.0f && Delta + Coins < 0.0f)
	{
		return false;
	}
	int OldCoins = Coins;
	Coins += Delta;
	MultiCastCoinChanged_Implementation(OldCoins);

	//OnCoinsChanged.Broadcast(this,Coins,Delta);
	return true;
}

void AACTPlayerState::MultiCastCreditChanged_Implementation(int32 OldState)
{
	OnCreditsChanged.Broadcast(this,Credits,Credits - OldState);
}

void AACTPlayerState::MultiCastCoinChanged_Implementation(int32 OldState)
{
	OnCoinsChanged.Broadcast(this,Coins,Coins - OldState);
}

int32 AACTPlayerState::GetCredits() const
{
	return Credits;
}

int32 AACTPlayerState::GetCoins() const
{
	return Coins;
}