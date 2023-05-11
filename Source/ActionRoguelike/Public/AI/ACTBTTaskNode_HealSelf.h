// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ACTBTTaskNode_HealSelf.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UACTBTTaskNode_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere,Category="Attribute")
	float HealValue;
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector AttackHealthKey;

public:
	UACTBTTaskNode_HealSelf();
};
