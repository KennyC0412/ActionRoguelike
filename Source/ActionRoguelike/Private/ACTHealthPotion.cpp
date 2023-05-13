// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTHealthPotion.h"
#include "ACTCharacter.h"
#include "ACTAttributeComponent.h"
#include "ACTCreditsComponent.h"
// Sets default values
AACTHealthPotion::AACTHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cost = 20;
	RespawnTime = 10.0f;
}

void AACTHealthPotion::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	return;
}

// Called when the game starts or when spawned
void AACTHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	BaseMesh->OnComponentBeginOverlap.AddDynamic(this,&AACTHealthPotion::OnActorOverlap);
}

// Called every frame
void AACTHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Yaw = Yaw + 2.0f;
	BaseMesh->SetWorldRotation(FRotator(0,Yaw,0));
}

void AACTHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!BaseMesh->IsVisible()) return;

	AACTCharacter* Character = Cast<AACTCharacter>(InstigatorPawn);

	if(Character)
	{
		UACTAttributeComponent* AttrComp = UACTAttributeComponent::GetAttributes(Character);
		UACTCreditsComponent* CreditsComp = UACTCreditsComponent::GetCredits(Character);
		if (AttrComp && !AttrComp->IsFull() && CreditsComp->IsEnoughCost(5))
		{
			AttrComp->ApplyHealthChange(this, 20);
			ResetVisibility(false);
			GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &AACTHealthPotion::ShowUp_Implementation, RespawnTime);
			CreditsComp->ApplyCoin(-5);
		}
	}
}
