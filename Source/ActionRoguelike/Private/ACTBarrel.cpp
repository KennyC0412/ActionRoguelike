// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AACTBarrel::AACTBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;
	//StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetSimulatePhysics(true);
	//StaticMeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	//StaticMeshComp->SetCollisionProfileName("PhysicsActor");

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 2000;
	RadialForceComp->ImpulseStrength = 5000;

}

// Called when the game starts or when spawned
void AACTBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AACTBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

