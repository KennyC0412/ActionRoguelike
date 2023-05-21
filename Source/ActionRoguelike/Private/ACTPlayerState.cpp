// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTPlayerState.h"

AACTPlayerState::AACTPlayerState()
{
	Coins = 0;
	Credits = 0;
}

void AACTPlayerState::AddCredits(int32 Delta)
{
	if(!ensure(Delta > 0.0f))
	{
		return;
	}
	Credits += Delta;
}

void AACTPlayerState::AddCoins(int32 Delta)
{
	if(!ensure(Delta > 0.0f))
	{
		return;
	}
	Coins += Delta;
	OnCoinsChanged.Broadcast(this,Coins,Delta);
}

bool AACTPlayerState::RemoveCoins(int32 Delta)
{
	if(!ensure(Delta > 0.0f))
	{
		return false;
	}

	if(Coins < Delta)
	{
		return false;
	}
	
	Coins -= Delta;
	OnCoinsChanged.Broadcast(this,Credits,Delta);
	return true;
}


int32 AACTPlayerState::GetCredits() const
{
	return Credits;
}

int32 AACTPlayerState::GetCoins() const
{
	return Coins;
}
