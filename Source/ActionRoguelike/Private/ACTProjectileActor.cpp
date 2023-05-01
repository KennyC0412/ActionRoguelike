// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTProjectileActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectile,All,All);

// Sets default values
AACTProjectileActor::AACTProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	/*SphereComp->SetCollisionObjectType(ECC_WorldStatic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);*/
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	ExplosionComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionComp");
	ExplosionComp->SetupAttachment(RootComponent);
	ExplosionComp->SetAutoActivate(false);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp->InitialSpeed = 1000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;

}

// Called when the game starts or when spawned
void AACTProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	//SphereComp->IgnoreActorWhenMoving(GetOwner(),true);
	SphereComp->OnComponentHit.AddDynamic(this,&AACTProjectileActor::OnProjectileHit);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle,this,&AACTProjectileActor::Destroy,5.0f);
}

void AACTProjectileActor::Destroy()
{
	Super::Destroy();
}

void AACTProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!GetWorld()) return;
	if(GetInstigator() != OtherActor)
	{
		UE_LOG(LogProjectile,Display,TEXT("Hit %s"),ToCStr(Hit.GetActor()->GetName()));
		ProjectileMovementComp->StopMovementImmediately();
		ExplosionComp->Activate();
		SphereComp->SetVisibility(false);
		EffectComp->SetVisibility(false);
	}
}

// Called every frame
void AACTProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

