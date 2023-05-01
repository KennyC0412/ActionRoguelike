// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTNormalProjectile.h"

#include "ACTAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogNormal,All,All)

AACTNormalProjectile::AACTNormalProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*SphereComp->SetCollisionObjectType(ECC_WorldStatic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);*/
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void AACTNormalProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->MoveIgnoreActors.Add(GetOwner());
	SphereComp->OnComponentHit.AddDynamic(this,&AACTNormalProjectile::OnActorHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AACTNormalProjectile::OnActorOverlap);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle,this,&AACTNormalProjectile::Destroy,5.0f);
}

void AACTNormalProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Hit.bBlockingHit)
	{
		ProjectileMovementComp->StopMovementImmediately();
		SphereComp->SetVisibility(false);
		EffectComp->SetVisibility(false);
		Explode();
		GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	}
}

void AACTNormalProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if(!OtherActor) return;
	if(OtherActor == GetOwner()) return;
	UACTAttributeComponent* AttributeComp = Cast<UACTAttributeComponent>(OtherActor->GetComponentByClass(UACTAttributeComponent::StaticClass()));
	if(!AttributeComp) return;
	AttributeComp->ApplyHealthChange(-20.0f);
	Destroy();
}