// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGameplayBase.h"

#include "Net/UnrealNetwork.h"


AACTGameplayBase::AACTGameplayBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	SetReplicates(true);
}

void AACTGameplayBase::OnRep_Interact()
{
	ResetVisibility(BaseMesh,bIsVisible);
}

void AACTGameplayBase::Interact_Implementation(APawn* InstigatorPawn)
{
	OnRep_Interact();
}

void AACTGameplayBase::ShowUp()
{
	ResetVisibility(BaseMesh,true);
}

void AACTGameplayBase::HideAndCooldownPowerup()
{
	ResetVisibility(BaseMesh,false);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&AACTGameplayBase::ShowUp,RespawnTime);
}

void AACTGameplayBase::ResetVisibility(UStaticMeshComponent* StaticMesh, bool isVisible)
{
	bIsVisible = isVisible;
	if(StaticMesh)
	{
		SetActorEnableCollision(bIsVisible);
		StaticMesh->SetVisibility(bIsVisible,true);
	}
}

void AACTGameplayBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACTGameplayBase,bIsVisible);
}