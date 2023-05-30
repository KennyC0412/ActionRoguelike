// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTCoin.h"

#include "ACTCharacter.h"
#include "ACTPlayerState.h"

// Sets default values
AACTCoin::AACTCoin()
{
}

// Called when the game starts or when spawned
void AACTCoin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AACTCoin::ServerInteract_Implementation(APawn* InstigatorPawn)
{
	if (!BaseMesh->IsVisible()) return;

	AACTCharacter* MyPlayer = Cast<AACTCharacter>(InstigatorPawn);
	if(MyPlayer)
	{
		AACTPlayerState* PS = Cast<AACTPlayerState>(MyPlayer->GetPlayerState());
		if(PS)
		{
			PS->ApplyCoins(5);
			HideAndCooldownPowerup();
			//Destroy();
		}
	}
}

void AACTCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	ServerInteract(InstigatorPawn);
}

// Called every frame
void AACTCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
