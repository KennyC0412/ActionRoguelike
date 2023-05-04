// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTHealthPotion.h"
#include "ACTCharacter.h"
#include "ACTAttributeComponent.h"
// Sets default values
AACTHealthPotion::AACTHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AACTHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACTHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACTHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!StaticMesh->IsVisible()) return;

	AACTCharacter* Character = Cast<AACTCharacter>(InstigatorPawn);

	if(Character)
	{
		UACTAttributeComponent* AttrComp = Character->GetAttrributeComp();
		if (AttrComp && !AttrComp->IsFull())
		{
			AttrComp->ApplyHealthChange(20);
			StaticMesh->SetVisibility(false);
			GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &AACTHealthPotion::ResetVisibility, 10.0f);
		}
	}
}

void AACTHealthPotion::ResetVisibility()
{
	StaticMesh->SetVisibility(true);

}
