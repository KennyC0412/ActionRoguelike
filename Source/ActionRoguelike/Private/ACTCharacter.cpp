// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTCharacter.h"

#include "ACTAttributeComponent.h"
#include "ACTInteractionComponent.h"
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

void AACTCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void AACTCharacter::NormalAttack_TimeElapsed()
{
	SpawnProjectile(NormalProjectileClass);
}

void AACTCharacter::MagicAttack_TimeElapsed()
{
	SpawnProjectile(MagicProjectileClass);
}

void AACTCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if(ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		//SpawnParams.Owner = this;
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		if(GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	
		FTransform SpawnTM = FTransform(ProjectileRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn,SpawnTM,SpawnParams);
	}
}

void AACTCharacter::NormalAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&AACTCharacter::NormalAttack_TimeElapsed,0.2f);
}

void AACTCharacter::MagicAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&AACTCharacter::MagicAttack_TimeElapsed,0.2f);
}

void AACTCharacter::DashAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&AACTCharacter::DashAttack_TimeElapsed,0.2f);
}

void AACTCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
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

	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AACTCharacter::NormalAttack);
	PlayerInputComponent->BindAction("MagicAttack",IE_Pressed,this,&AACTCharacter::MagicAttack);
	PlayerInputComponent->BindAction("Dash",IE_Pressed,this,&AACTCharacter::DashAttack);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AACTCharacter::Jump);
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

