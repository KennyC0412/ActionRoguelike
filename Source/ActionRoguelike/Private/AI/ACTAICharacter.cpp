// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTAICharacter.h"

#include "ACTActionComponent.h"
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
	ActionComp = CreateDefaultSubobject<UACTActionComponent>("ActionComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	TimeToHitParamName = "TimeToHit";
	TargeActorKey = "TargetActor";
}

void AACTAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargeActorKey,NewTarget);
	}
}

AActor* AACTAICharacter::GetTargetActor() const
{
	AAIController* AIController = Cast<AAIController>(GetController());
    if(AIController)
    {
    	return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargeActorKey));
    }
	return nullptr;
}

void AACTAICharacter::OnPawnSeen(APawn* Pawn)
{
	if(GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		MulticastOnPawnSeen();
	}
}

void AACTAICharacter::MulticastOnPawnSeen_Implementation()
{
	if(SpottedWidget == nullptr)
	{
		SpottedWidget = CreateWidget<UACTWorldUserWidget>(GetWorld(),SpottedWidgetClass);
	}
	if(SpottedWidget && !SpottedWidget->IsInViewport())
	{
		SpottedWidget->SetVisibility(ESlateVisibility::Visible);
		SpottedWidget->AttachedActor = this;
		SpottedWidget->AddToViewport();
		FTimerDelegate Delegate;
		Delegate.BindUFunction(SpottedWidget,"SetVisibility",ESlateVisibility::Hidden);
		GetWorldTimerManager().SetTimer(SpottedHidenHandle,Delegate,1.0f,false);
	}
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
