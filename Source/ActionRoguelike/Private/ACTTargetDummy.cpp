// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTTargetDummy.h"

DEFINE_LOG_CATEGORY_STATIC(LogTarget,All,All)

// Sets default values
AACTTargetDummy::AACTTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;
	
	AttributeComp = CreateDefaultSubobject<UACTAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this,&AACTTargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void AACTTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AACTTargetDummy::OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth,
	float Delta)
{
		UE_LOG(LogTarget,Display,TEXT("set material param"));
		StaticMeshComp->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
}

// Called every frame
void AACTTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

