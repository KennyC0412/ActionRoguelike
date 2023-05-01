// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTBarrel.h"

#include "DrawDebugHelpers.h"
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
	RadialForceComp->Radius = 700;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->ImpulseStrength = 1500;

}

// Called when the game starts or when spawned
void AACTBarrel::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->OnComponentHit.AddDynamic(this,&AACTBarrel::OnActorHit);
	
}

// Called every frame
void AACTBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACTBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
	FString CombinedString(TEXT("Boom!"));
	DrawDebugString(GetWorld(),Hit.ImpactPoint,CombinedString,nullptr,FColor::Green,2.0f,false);
}

