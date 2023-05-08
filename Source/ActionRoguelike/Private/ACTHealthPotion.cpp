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

	RespawnTime = 10.0f;
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
	Yaw = Yaw + 2.0f;
	StaticMesh->SetWorldRotation(FRotator(0,Yaw,0));
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
			AttrComp->ApplyHealthChange(this, 20);
			ResetVisibility(false);
			GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &AACTHealthPotion::ShowUp, RespawnTime);
		}
	}
}

void AACTHealthPotion::ShowUp()
{
	ResetVisibility(true);
}

void AACTHealthPotion::ResetVisibility(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive,true);
}
