// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AACTDashProjectile::AACTDashProjectile()
{
	ProjectileMovementComp->InitialSpeed = 6000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	
}


void AACTDashProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetInstigator()) return;
	Explode_Implementation();
}

void AACTDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->MoveIgnoreActors.Add(GetInstigator());
	GetWorldTimerManager().SetTimer(TimerHandle,this,&AACTDashProjectile::Explode_Implementation,0.2f);
}

void AACTDashProjectile::TelePort()
{
    APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    Pawn->SetActorLocation(GetActorLocation());
	Destroy();
}

void AACTDashProjectile::Explode_Implementation()
{
	if(ensure(!IsPendingKill()))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		GetWorldTimerManager().SetTimer(TimerHandle,this,&AACTDashProjectile::TelePort,0.2f);
		ProjectileMovementComp->StopMovementImmediately();
	}
}


