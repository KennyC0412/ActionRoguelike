// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ACTActionComponent.generated.h"


class UACTActionEffect;
class UACTAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UACTActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	void AddAction(AActor* Instigator, TSubclassOf<UACTAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable)
	void RemoveAction(UACTAction* ActionToMove);

	UACTActionComponent();

protected:

	UFUNCTION(Server,Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);
	
	UPROPERTY(EditAnywhere, Category="Action")
	TArray<TSubclassOf<UACTAction>> DefaultActions;

	
	UPROPERTY(Replicated)
	TArray<UACTAction*> Actions;
	
	virtual void BeginPlay() override;

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool HasAction(TSubclassOf<UACTAction> ActionClass);
		
};
