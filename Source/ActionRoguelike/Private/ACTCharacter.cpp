// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTCharacter.h"

#include "ACTAttributeComponent.h"
#include "ACTInteractionComponent.h"
#include "ACTActionComponent.h"
#include "EngineUtils.h"
#include "AI/ACTAICharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AACTCharacter::AACTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UACTInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UACTAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UACTActionComponent>("ActionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AACTCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0;
	AddMovementInput(ControlRot.Vector(),value);
}

void AACTCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0;
	
	FVector RightVecator = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVecator,value);
}

void AACTCharacter::SprintStart()
{
	ActionComp->StartActionByName(this,"Sprint");
}

void AACTCharacter::SprintStop()
{
	ActionComp->StopActionByName(this,"Sprint");
}

void AACTCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this,"PrimaryAttack");
}

void AACTCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this,"BlackHole");
}

void AACTCharacter::DashAttack()
{
	ActionComp->StartActionByName(this,"Dash");
}

void AACTCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void AACTCharacter::Parry()
{
	ActionComp->StartActionByName(this,"Parry");
}

FVector AACTCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveRight",this,&AACTCharacter::MoveRight);

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AACTCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("MagicAttack",IE_Pressed,this,&AACTCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash",IE_Pressed,this,&AACTCharacter::DashAttack);
	PlayerInputComponent->BindAction("Parry",IE_Pressed,this,&AACTCharacter::Parry);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AACTCharacter::Jump);
	
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AACTCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AACTCharacter::SprintStop);
	
	PlayerInputComponent->BindAction("Execute",IE_Pressed,InteractionComp,&UACTInteractionComponent::PrimaryInteract);

}

void AACTCharacter::OnHealthChanged(AActor* OtherActor, UACTAttributeComponent* OtherComp, float NewHealth, float Delta)
{
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}

void AACTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this,&AACTCharacter::OnHealthChanged);
}


void AACTCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}

void AACTCharacter::KillSelf()
{
	AttributeComp->Kill(this);			
}

