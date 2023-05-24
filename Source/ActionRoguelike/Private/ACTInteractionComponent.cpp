// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTInteractionComponent.h"

#include "ACTGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "ACTWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarEnableDebugDraw(TEXT("act.EnableDebugDraw"),false,TEXT("Enable draw debug line and sphere for interact component"),ECVF_Cheat);

DEFINE_LOG_CATEGORY_STATIC(LogInteraction, All, All)

// Sets default values for this component's properties
UACTInteractionComponent::UACTInteractionComponent()
{
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void UACTInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarEnableDebugDraw.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	/*FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> HitArray;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitArray,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Blue;

	//clear before try to set
	FocusedActor = nullptr;
	
	for(FHitResult HitResult : HitArray)
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor)
		{
			if(bDebugDraw)
			{
				DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,TraceRadius,32,LineColor,false,2.0f);
			}
			if(HitActor->Implements<UACTGameplayInterface>())
			{
				FocusedActor = HitActor;
				UE_LOG(LogTemp, Log, TEXT("Focus"));
				break;
			}
		}
	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && DefaultWidgetClass)
		{
			DefaultWidgetInstance = CreateWidget<UACTWorldUserWidget>(GetWorld(),DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,3.0f,0,5.0f);
	}
}


// Called when the game starts
void UACTInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->GetWorldTimerManager().SetTimer(Timer_FindInteraction,this,&UACTInteractionComponent::FindBestInteractable,0.1f,true);
	// ...
	
}


void UACTInteractionComponent::PrimaryInteract()
{
	if(FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,"No Focus Actor to interact.");
		return;
	}
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	IACTGameplayInterface::Execute_Interact(FocusedActor,InstigatorPawn);
}

void UACTInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
