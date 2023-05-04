// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AACTProjectileBase::AACTProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp->InitialSpeed = 5000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;

	ImpactVFX = CreateDefaultSubobject<UParticleSystem>("ImpactVFX");
	CastSpellVFX = CreateDefaultSubobject<UParticleSystem>("CastSpellVFX");
	
	ProjectileLoopAudioComp = CreateDefaultSubobject<UAudioComponent>("ProjectileLoopAudioComp");
	ProjectileLoopAudioComp->SetupAttachment(RootComponent);

	/*ProjectileImpactAudioComp = CreateDefaultSubobject<UAudioComponent>("ProjectileImpactAudioComp");
	ProjectileImpactAudioComp->SetupAttachment(RootComponent);
	ProjectileImpactAudioComp->bAutoActivate = false;*/
}

void AACTProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AACTProjectileBase::Explode_Implementation()
{
	if(ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		ProjectileLoopAudioComp->Deactivate();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),SoundBase,GetActorLocation());
		Destroy();
	}
}

void AACTProjectileBase::Destroy()
{
	Super::Destroy();
}

// Called every frame
void AACTProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

