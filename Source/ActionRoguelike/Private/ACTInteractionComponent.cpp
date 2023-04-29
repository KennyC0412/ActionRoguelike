// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTInteractionComponent.h"

#include "ACTGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteraction, All, All)

// Sets default values for this component's properties
UACTInteractionComponent::UACTInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACTInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UACTInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End;
	End = EyeLocation + (EyeRotation.Vector() * 200);
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	DrawDebugLine(GetWorld(),EyeLocation,End,FColor::Red,false,3.0f,0,5);
	AActor* HitActor = Hit.GetActor();
	if(HitActor)
	{
		if(HitActor->Implements<UACTGameplayInterface>())
		{
			APawn* InstigatorPawn = Cast<APawn>(Owner);
			UE_LOG(LogInteraction,Display,TEXT("%s"),ToCStr(HitActor->GetName()));
			IACTGameplayInterface::Execute_Interact(HitActor,InstigatorPawn);
		}
	}
	
}

// Called every frame
void UACTInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
