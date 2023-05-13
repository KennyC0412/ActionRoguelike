// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTAICharacter.h"

#include "ACTAttributeComponent.h"
#include "ACTWorldUserWidget.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AACTAICharacter::AACTAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UACTAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	TimeToHitParamName = "TimeToHit";
}

void AACTAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if(ensure(AIController))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
	}
}

void AACTAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(),GetActorLocation(),"Player Spotted",nullptr,FColor::White,4.0f,true);
}

void AACTAICharacter::OnHealthChanged(AActor* InstigatorActor, UACTAttributeComponent* OtherComp, float NewHealth, float Delta)
{
	
	if(Delta < 0.0f)
	{
		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UACTWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
            if(ActiveHealthBar)
            {
            	ActiveHealthBar->AttachedActor = this;
            	ActiveHealthBar->AddToViewport();
            }
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
		//GetMesh()->SetScalarParameterValueOnMaterials("HitFlashSpeed",10);
		
		if(NewHealth <= 0.0f)
		{
			//Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			//Set lifespan
			SetLifeSpan(5.0f);
		}
	}
}

void AACTAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&AACTAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&AACTAICharacter::OnHealthChanged);
}




