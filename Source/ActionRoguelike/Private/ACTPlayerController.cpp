// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTPlayerController.h"

#include "Blueprint/UserWidget.h"

void AACTPlayerController::TogglePauseMenu()
{
	if(PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this,PauseMenuClass);
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AACTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AACTPlayerController::TogglePauseMenu);
}

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
