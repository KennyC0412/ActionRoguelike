// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTMagicProjectile.h"

#include "ACTProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectile, All, All);

// Sets default values
AACTMagicProjectile::AACTMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*SphereComp->SetCollisionObjectType(ECC_WorldStatic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);*/
	SphereComp->SetCollisionProfileName("BlackHole");
	
	ProjectileMovementComp->InitialSpeed = 300.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 1000.0f;
	RadialForceComponent->RemoveObjectTypeToAffect(TEnumAsByte<EObjectTypeQuery>(ECC_Pawn));
	RadialForceComponent->ImpulseStrength = -500.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bIgnoreOwningActor = true;
}

// Called when the game starts or when spawned
void AACTMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
	SphereComp->OnComponentHit.AddDynamic(this,&AACTMagicProjectile::OnActorHit);
	
	GetWorldTimerManager().SetTimer(DestroyTimerHandle,this,&AACTMagicProjectile::Destroy,5.0f);
}

void AACTMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}



void AACTMagicProjectile::Explode_Implementation()
{
	
}


// Called every frame
void AACTMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RadialForceComponent->FireImpulse();
}

