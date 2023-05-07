// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AACTAIController::BeginPlay()
{
	Super::BeginPlay();
	if(ensureMsgf(BehaviorTree,TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}	

	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0);
	if(MyPawn)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	}*/
}
