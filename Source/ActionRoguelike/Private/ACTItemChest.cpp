// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTItemChest.h"

#include "Particles/ParticleSystemComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogChest, All, All)

// Sets default values
AACTItemChest::AACTItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoldMesh");
	GoldMesh->SetupAttachment(RootComponent);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(GoldMesh);
	TargetPitch = 110.0f;
}

void AACTItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if(IsClosed)
	{
		LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
		ParticleSystemComp->Activate();
		IsClosed = false;
	}
	else
	{
		LidMesh->SetRelativeRotation(FRotator(0,0,0));
		IsClosed = true;
	}
}



// Called when the game starts or when spawned
void AACTItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACTItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

