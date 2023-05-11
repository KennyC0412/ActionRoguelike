// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTBTService_CheckLowHealth.h"

#include "ACTAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UACTBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AAIController* AIC = OwnerComp.GetAIOwner();
		if(AIC)
		{
			APawn* AIPawn = AIC->GetPawn();
			UACTAttributeComponent* AttributeComp = UACTAttributeComponent::GetAttributes(AIPawn);
			if(AttributeComp->IsLowHealth())
			{
				AIC->ClearFocus(EAIFocusPriority::Move);
				BlackboardComp->SetValueAsBool(AttackHealthKey.SelectedKeyName,true);
			}
		}
	}
}
