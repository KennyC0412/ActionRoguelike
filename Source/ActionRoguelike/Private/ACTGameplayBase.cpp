// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameplayBase.h"


AACTGameplayBase::AACTGameplayBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
}

void AACTGameplayBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

void AACTGameplayBase::ShowUp_Implementation()
{
	ResetVisibility(true);
}

void AACTGameplayBase::Hide_Implementation()
{
	ResetVisibility(false);
}

void AACTGameplayBase::ResetVisibility(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive,true);
}
