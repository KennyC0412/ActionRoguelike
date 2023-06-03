// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTItemChest.h"

#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogChest, All, All)

// Sets default values
AACTItemChest::AACTItemChest()
{
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoldMesh");
	GoldMesh->SetupAttachment(RootComponent);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(GoldMesh);
	TargetPitch = 110.0f;

	SetReplicates(true);
}

void AACTItemChest::OnRep_LidOpened()
{
	float CurPitch = IsClosed ? 0.0f : TargetPitch;
	
	LidMesh->SetRelativeRotation(FRotator(CurPitch,0,0));
	if(CurPitch > 0.0f)
	{
		ParticleSystemComp->Activate();
	}
}

void AACTItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IsClosed = !IsClosed;
	OnRep_LidOpened();
}

void AACTItemChest::OnActorLoaded_Implementation()
{
	Super::OnActorLoaded_Implementation();
	OnRep_LidOpened();
}

void  AACTItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACTItemChest, IsClosed);
}

