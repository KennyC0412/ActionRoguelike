// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTBTTaskNode_RangedAttack.h"

#include "ACTAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UACTBTTaskNode_RangedAttack::UACTBTTaskNode_RangedAttack()
{
	MaxBulletsSpread = 5.0f;
}


EBTNodeResult::Type UACTBTTaskNode_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if(ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if(!MyPawn)
		{
			return EBTNodeResult::Failed;
		}
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if(TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if(!UACTAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		
		MyController->SetFocus(TargetActor);

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f,MaxBulletsSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletsSpread,MaxBulletsSpread);
		
		FActorSpawnParameters Params;
		Params.Instigator = MyPawn;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

