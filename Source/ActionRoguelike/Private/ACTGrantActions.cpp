// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTGrantActions.h"

#include "ACTAction.h"
#include "ACTActionComponent.h"
#include "ACTCharacter.h"

void AACTGrantActions::Interact_Implementation(APawn* InstigatorPawn)
{
	AACTCharacter* MyPlayer = Cast<AACTCharacter>(InstigatorPawn);
	if(MyPlayer)
	{
		UACTActionComponent* ActionComponent = MyPlayer->GetActionComp();
		if(ActionComponent)
		{
			for(TSubclassOf<UACTAction> ActionClass : CanGainActions)
            {
				if(!ActionComponent->HasAction(ActionClass))
				{
					ActionComponent->AddAction(MyPlayer,ActionClass);
				}
            }
		}
	}
}

AACTGrantActions::AACTGrantActions()
{
	
}

void AACTGrantActions::BeginPlay()
{
}
