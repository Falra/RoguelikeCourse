// Fill out your copyright notice in the Description page of Project Settings.


#include "VDCharacter.h"
#include "DrawDebugHelpers.h"
#include "VDActionComponent.h"
#include "VDInteractionComponent.h"
#include "VDAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVDCharacter::AVDCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	InteractionComponent = CreateDefaultSubobject<UVDInteractionComponent>("InteractionComponent");
	
	AttributeComponent = CreateDefaultSubobject<UVDAttributeComponent>("AttributeComponent");

	ActionComponent = CreateDefaultSubobject<UVDActionComponent>("ActionComponent");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
	TimeToHitParamName = "TimeToHit";
}

void AVDCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AVDCharacter::OnHealthChanged);
}

// Called to bind functionality to input

void AVDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVDCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVDCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AVDCharacter::PrimaryAttack);

	// Used generic name 'SecondaryAttack' for binding
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AVDCharacter::BlackHoleAttack);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AVDCharacter::Dash);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AVDCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AVDCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AVDCharacter::SprintStop);

}

void AVDCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount /* = 100 */);
}

void AVDCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	AddMovementInput(ControlRotation.Vector(), Value);
}

void AVDCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AVDCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AVDCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void AVDCharacter::PrimaryAttack()
{	
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AVDCharacter::BlackHoleAttack()
{
	ActionComponent->StartActionByName(this, "BlackHoleAttack");
}

void AVDCharacter::Dash()
{
	ActionComponent->StartActionByName(this, "Dash");
}

void AVDCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void AVDCharacter::OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth,
                                   float DeltaHealth)
{
	// Damaged
	if (DeltaHealth < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(DeltaHealth);
		AttributeComponent->ApplyRage(InstigatorActor, RageDelta);
	}

	// Died
	if (NewHealth <= 0.0f && DeltaHealth < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

FVector AVDCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}
