// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTInteractionComponent.h"

#include "ACTGameplayInterface.h"
#include "DrawDebugHelpers.h"


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

	FVector End = EyeLocation + (EyeRotation.Vector() * 200);

	/*FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> HitArray;

	float Radius = 30.0f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitArray,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Blue;

	for(FHitResult HitResult : HitArray)
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor)
		{
			DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,Radius,32,LineColor,false,2.0f);
			if(HitActor->Implements<UACTGameplayInterface>())
			{
				APawn* InstigatorPawn = Cast<APawn>(Owner);
				UE_LOG(LogInteraction,Display,TEXT("%s"),*GetNameSafe(HitActor));
				IACTGameplayInterface::Execute_Interact(HitActor,InstigatorPawn);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,3.0f,0,5.0f);
}

// Called every frame
void UACTInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
