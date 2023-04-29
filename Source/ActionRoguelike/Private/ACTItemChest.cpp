// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTItemChest.h"


DEFINE_LOG_CATEGORY_STATIC(LogChest,All,All)

// Sets default values
AACTItemChest::AACTItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110.0f;
}

void AACTItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if(IsClosed)
	{
		LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
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

