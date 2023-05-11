// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ACTBTTaskNode_HealSelf.h"

#include "ACTAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UACTBTTaskNode_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AAIController* AIC = OwnerComp.GetAIOwner();
		if(AIC)
		{
			APawn* AIPawn = AIC->GetPawn();
			if(AIPawn)
			{
				UACTAttributeComponent* AttributeComp = UACTAttributeComponent::GetAttributes(AIPawn);
				if(AttributeComp && AttributeComp->IsAlive())
				{
					AttributeComp->ApplyHealthChange(AIPawn,HealValue);
					BlackboardComp->SetValueAsBool(AttackHealthKey.SelectedKeyName,false);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

UACTBTTaskNode_HealSelf::UACTBTTaskNode_HealSelf()
{
	HealValue = 60.0f;
}
