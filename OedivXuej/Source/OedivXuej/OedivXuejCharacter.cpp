// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OedivXuejCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogMyGame);

//////////////////////////////////////////////////////////////////////////
// AOedivXuejCharacter

AOedivXuejCharacter::AOedivXuejCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("Create"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Initialize stats for our character
	Health = 1.0f;
	Energy = 1.0f;
	HealthPercent = FString::FromInt(Health * 100);
	EnergyPercent = FString::FromInt(Energy * 100);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	firstRight = true;
	firstForward = true;

	UE_LOG(LogMyGame, Warning, TEXT("Hello"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOedivXuejCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AOedivXuejCharacter::JumpRoll);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AOedivXuejCharacter::Roll);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOedivXuejCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOedivXuejCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AOedivXuejCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AOedivXuejCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AOedivXuejCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AOedivXuejCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AOedivXuejCharacter::OnResetVR);

	Animation = Cast<UAnimationCharacter>(GetMesh()->GetAnimInstance());
	Animation->NativeInitializeAnimation();
}

void AOedivXuejCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AOedivXuejCharacter::RefillEnergy()
{
	if (Energy < 1.0)
	{
		Energy += 0.05;
		UpdateEnergyPercent();
	}
}

void AOedivXuejCharacter::UpdateEnergyPercent()
{
	EnergyPercent = FString::FromInt(Energy * 100);
}

void AOedivXuejCharacter::UpdateHealthPercent()
{
	HealthPercent = FString::FromInt(Health * 100);
}

void AOedivXuejCharacter::Roll()
{
	if (!Animation->IsRolling) {
		if (Energy >= 0.25)
		{
			Animation->IsRolling = true;
			Energy -= 0.25;
			UpdateEnergyPercent();
			Info = "";
		}
		else
			Info = "Not Enought Energy !";
	}
	//RemoveActionMapping
	/*float Value = 10.5f;
	 find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get forward vector
	 const FVector Direction = GetActorForwardVector();
	 this->AddMovementInput(Direction, Value);*/

}

void AOedivXuejCharacter::JumpRoll()
{
	if (!Animation->IsRolling)
	{
			if (Energy >= 0.20) {
				Info = "";
				Jump();
				Energy -= 0.20;
				UpdateEnergyPercent();
			}
			else
				Info = "Not Enought Energy !";
	}
}

void AOedivXuejCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AOedivXuejCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AOedivXuejCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AOedivXuejCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AOedivXuejCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AOedivXuejCharacter::MoveForward(float Value)
{
	FVector newLocation = this->GetActorLocation();

	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (firstForward)
		{
			firstForward = false;
		}
		else if (precForward != newLocation)
		{
			FVector test = newLocation - precForward;

			if (test.X * GetPendingMovementInputVector().X < 0 && test.Y * GetPendingMovementInputVector().Y < 0) {
				UE_LOG(LogMyGame, Warning, TEXT("%s, %s"), *GetPendingMovementInputVector().ToString(), *test.ToString());
				UE_LOG(LogMyGame, Warning, TEXT("Not Pass"));
			}
		}

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	precForward = this->GetActorLocation();
}

void AOedivXuejCharacter::MoveRight(float Value)
{
	FVector newLocation = this->GetActorLocation();

	if ( (Controller != NULL) && (Value != 0.0f) )
	{

		if (firstRight)
		{
			firstRight = false;
		}
		else if (precRight != newLocation)
		{
			FVector test = newLocation - precRight;

			if (test.X * GetPendingMovementInputVector().X < 0 && test.Y * GetPendingMovementInputVector().Y < 0) {
				UE_LOG(LogMyGame, Warning, TEXT("%s, %s"), *GetPendingMovementInputVector().ToString(), *test.ToString());
				UE_LOG(LogMyGame, Warning, TEXT("Not Pass"));
			}
		}

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

	}

	precRight = this->GetActorLocation();

}
