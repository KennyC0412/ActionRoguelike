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

void AACTCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	AACTCharacter* MyPlayer = Cast<AACTCharacter>(InstigatorPawn);
	if(MyPlayer)
	{
		AACTPlayerState* PS = Cast<AACTPlayerState>(MyPlayer->GetPlayerState());
		if(PS)
		{
			PS->AddCoins(5);
			Destroy();
		}
	}
}

// Called every frame
void AACTCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
