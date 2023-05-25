// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTAction_Projectile.h"

#include "ACTAttributeComponent.h"
#include "ACTCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UACTAction_Projectile::UACTAction_Projectile()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
	RageCost = 0.0f;
}

void UACTAction_Projectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(Character)
	{
		UACTAttributeComponent* AttributeComponent = UACTAttributeComponent::GetAttributes(Instigator);
		if(AttributeComponent && RageCost > AttributeComponent->GetRage())
		{
			AACTCharacter* MyPlayer = Cast<AACTCharacter>(Instigator);
			if(MyPlayer)
			{
				USoundBase* Sound = MyPlayer->GetRandomSound();
				if(Sound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(),Sound,MyPlayer->GetActorLocation());
				}
			}
			StopAction(Instigator);
			return;
		}
		AttributeComponent->ReduceRage(RageCost);
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastSpellVFX,Character->GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);
		
		FTimerHandle TimerHandle_AttackDelay;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this,"AttackDelay_Elapsed",Character);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,TimerDelegate,AttackAnimDelay,false);
	}

}

void UACTAction_Projectile::AttackDelay_Elapsed(ACharacter* InstigatorActor)
{
	if(ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorActor->GetMesh()->GetSocketLocation(HandSocketName);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorActor;
		//SpawnParams.Owner = this;
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorActor);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FVector TraceStart = InstigatorActor->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorActor->GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		if(GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	
		FTransform SpawnTM = FTransform(ProjectileRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParams);
	}

	StopAction(InstigatorActor);
}