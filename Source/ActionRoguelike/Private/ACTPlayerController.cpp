// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTPlayerController.h"

void AACTPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void AACTPlayerController::BeginPlayingState()
{
	//Super::BeginPlayingState();
	BlueprintBeginPlayingState();
}

void AACTPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	OnPlayerStateReceived.Broadcast(PlayerState);
}
