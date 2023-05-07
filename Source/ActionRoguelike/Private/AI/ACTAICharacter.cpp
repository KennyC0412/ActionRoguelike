// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTAICharacter.h"

#include "ACTAttributeComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AACTAICharacter::AACTAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UACTAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AACTAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if(ensure(AIController))
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		BlackboardComponent->SetValueAsObject("TargetActor",Pawn);
		DrawDebugString(GetWorld(),GetActorLocation(),"Player Spotted",nullptr,FColor::White,4.0f,true);
	}
}

void AACTAICharacter::OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
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
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
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




