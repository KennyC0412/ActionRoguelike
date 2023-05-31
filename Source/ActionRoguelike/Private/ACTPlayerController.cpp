// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTPlayerController.h"

void AACTPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
