// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AACTCharacter::AACTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AACTCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(),value);
}

void AACTCharacter::MoveRight(float value)
{
	//AddMovementInput(GetACtor(),value);
}

// Called every frame
void AACTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AACTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&AACTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("MoveRight",this,&AACTCharacter::MoveRight);

}

