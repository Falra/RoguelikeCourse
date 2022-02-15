// Fill out your copyright notice in the Description page of Project Settings.


#include "VDCharacter.h"
#include "DrawDebugHelpers.h"
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
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
	TimeToHitParamName = "TimeToHit";
	HandSocketName = "Muzzle_01";
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

void AVDCharacter::PrimaryAttack()
{
	StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AVDCharacter::PrimaryAttack_TimeElapsed, TimerAttack_InRate);
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);

}

void AVDCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}


void AVDCharacter::BlackHoleAttack()
{
	StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &AVDCharacter::BlackholeAttack_TimeElapsed, TimerAttack_InRate);
}


void AVDCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}


void AVDCharacter::Dash()
{
	StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &AVDCharacter::Dash_TimeElapsed, TimerAttack_InRate);
}

void AVDCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnimation);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

}

void AVDCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void AVDCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FHitResult Hit;
		FVector TraceStart = CameraComponent->GetComponentLocation();
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = CameraComponent->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FRotator ProjRotation;
		// true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Adjust location to end up at crosshair look-at
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			// Fall-back since we failed to find any blocking hit
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}


		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void AVDCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void AVDCharacter::OnHealthChanged(AActor* InstigatorActor, UVDAttributeComponent* OwningComponent, float NewHealth,
                                   float DeltaHealth)
{
	if (DeltaHealth < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
	
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
