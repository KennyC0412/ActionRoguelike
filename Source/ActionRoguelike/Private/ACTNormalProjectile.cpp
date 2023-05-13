// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTNormalProjectile.h"

#include "ACTAttributeComponent.h"
#include "ACTCharacter.h"
#include "ACTGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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

	SphereComp->MoveIgnoreActors.Add(GetInstigator());
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);

	SphereComp->OnComponentHit.AddDynamic(this,&AACTNormalProjectile::OnActorHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AACTNormalProjectile::OnActorOverlap);

	AACTCharacter* Character = Cast<AACTCharacter>(GetInstigator());
	if(Character && CastSpellVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),CastSpellVFX,Character->GetMesh()->GetSocketLocation(Character->HandSocketName));
	}
	GetWorldTimerManager().SetTimer(DestroyTimerHandle,this,&AACTNormalProjectile::Destroy,5.0f);
}

void AACTNormalProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!OtherActor) return;
	if(OtherActor == GetInstigator()) return;
	if(Hit.bBlockingHit)
	{
		UE_LOG(LogNormal,Display,TEXT("Hit : %s"),*(OtherActor->GetName()));
		ProjectileMovementComp->StopMovementImmediately();
		UGameplayStatics::PlayWorldCameraShake(GetWorld(),CameraShake,Hit.ImpactPoint,0.0f,500.0f);
		UACTAttributeComponent* AttributeComp = UACTAttributeComponent::GetAttributes(OtherActor);
		if(AttributeComp)
		{
			AttributeComp->ApplyHealthChange(OtherActor,-DamageCount);
		}
		Explode();
		GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	}
}

void AACTNormalProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(!OtherActor) return;
	if(OtherActor == GetInstigator()) return;
	/*UE_LOG(LogNormal,Display,TEXT("Overlap : %s"),*(OtherActor->GetName()));

	UACTAttributeComponent* AttributeComp = Cast<UACTAttributeComponent>(OtherActor->GetComponentByClass(UACTAttributeComponent::StaticClass()));
	if(!AttributeComp) return;
	AttributeComp->ApplyHealthChange(OtherActor,-DamageCount);*/
	if(UACTGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,DamageCount,SweepResult))
	{
		Explode();
	}
	//Destroy();
}