// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OedivXuejCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogMyGame);

//////////////////////////////////////////////////////////////////////////
// AOedivXuejCharacter

AOedivXuejCharacter::AOedivXuejCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("Create"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bReplicates = true;

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

	canRoll = true;
	canStab = true;
	canStabHigh = true;
	canStabJump = true;

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

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AOedivXuejCharacter::OnRoll);
	PlayerInputComponent->BindAction("Stab", IE_Pressed, this, &AOedivXuejCharacter::OnStab);
	PlayerInputComponent->BindAction("StabJump", IE_Pressed, this, &AOedivXuejCharacter::OnStabJump);
	PlayerInputComponent->BindAction("StabHigh", IE_Pressed, this, &AOedivXuejCharacter::OnStabHigh);

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

}

void AOedivXuejCharacter::JumpRoll()
{
	if (this->GetCharacterMovement()->Velocity.Z == 0 && canMove())
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

void AOedivXuejCharacter::RefillEnergy()
{
	if (Energy < 1.0)
	{
		Energy += 0.005;
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

void AOedivXuejCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (!canRoll) {
		FVector direction = GetActorForwardVector() * 10.f;
		AddActorWorldOffset(direction, true);
	}
	else if (!canStabHigh) {
		FVector direction = GetActorForwardVector() * 8.f;
		AddActorWorldOffset(direction, true);
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(RollAnimation)) {
		canRoll = true;
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(StabAnimation)) {
		canStab = true;
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(StabHighAnimation)) {
		canStabHigh = true;
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(StabJumpAnimation)) {
		canStabJump = true;
	}


	/*
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(firstStabAnimation) && continueStab && numberStab == 0) {
	OnStab2(secondStabAnimation);
	continueStab = false;
	numberStab += 1;
	}
	else if (numberStab > 0 && !canStab) {
	FVector direction = GetActorForwardVector() * 1.5f;
	AddActorWorldOffset(direction, true);
	}
	else if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(secondStabAnimation) && numberStab == 1) {
	continueStab = false;
	canStab = true;
	numberStab = 0;
	moveStab = true;
	}
	else if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(secondStabAnimation) && continueStab && numberStab == 1) {
	OnStab2(thirdStabAnimation);
	continueStab = false;
	numberStab += 1;
	}
	else if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(secondStabAnimation) && !continueStab && numberStab == 1) {
	continueStab = false;
	canStab = true;
	numberStab = 0;
	moveStab = true;
	}
	else if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(thirdStabAnimation) && numberStab == 2) {
	continueStab = false;
	canStab = true;
	numberStab = 0;
	moveStab = true;
	}*/
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

	if ((Controller != NULL) && (Value != 0.0f) && canMove())
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

	if ( (Controller != NULL) && (Value != 0.0f) && canMove())
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

bool AOedivXuejCharacter::canMove() {
	return canStab && canRoll && canStabHigh && canStabJump;
}

void AOedivXuejCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AOedivXuejCharacter::MultiCastSetRoll_Implementation()
{
	SetRolling();
}

void AOedivXuejCharacter::ServerSetRoll_Implementation()
{
	MultiCastSetRoll();
}

bool AOedivXuejCharacter::ServerSetRoll_Validate()
{
	return true;
}

void AOedivXuejCharacter::OnRoll()
{
	if (Energy >= 0.25)
	{
		if (canRoll){
			Energy -= 0.25;
			UpdateEnergyPercent();
			Info = "";
			if (HasAuthority())
			{
				MultiCastSetRoll();
			}
			else
			{
				ServerSetRoll();
			}
		}
	}
	else
		Info = "Not Enought Energy !";
}

void AOedivXuejCharacter::SetRolling()
{
	// try and play a firing animation if specified
	if (RollAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			canRoll = false;
			AnimInstance->Montage_Play(RollAnimation, 1.6f);
		}
	}
}

// Animation Stab
void AOedivXuejCharacter::MultiCastSetStab_Implementation(UAnimMontage* animStab)
{
	SetStab(animStab);
}

void AOedivXuejCharacter::ServerSetStab_Implementation(UAnimMontage* animStab)
{
	MultiCastSetStab(animStab);
}

bool AOedivXuejCharacter::ServerSetStab_Validate(UAnimMontage* animStab)
{
	return true;
}

void AOedivXuejCharacter::OnStab()
{
	if (Energy >= 0.05)
	{

		if (canMove()) {
			Energy -= 0.05;
			UpdateEnergyPercent();
			Info = "";
			if (HasAuthority())
			{
				MultiCastSetStab(StabAnimation);
			}
			else
			{
				ServerSetStab(StabAnimation);
			}
		}
		/*else if (!canStab) {
		continueStab = true;
		}*/
	}
	else
		Info = "Not Enought Energy !";
}

/*
void AOedivXuejCharacter::OnStab2(UAnimMontage* animStab)
{
if (Energy >= 0.05)
{
Energy -= 0.05;
UpdateEnergyPercent();
Info = "";
if (HasAuthority())
{
SetStab(animStab);
}
else
{
ServerSetStab(animStab);
}
}
else
Info = "Not Enought Energy !";
}*/


void AOedivXuejCharacter::SetStab(UAnimMontage* animStab)
{
	// try and play a firing animation if specified
	if (animStab != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(animStab, 1.5f);
			canStab = false;
		}
	}
}

//Attack High Stab
void AOedivXuejCharacter::MultiCastSetStabHigh_Implementation()
{
	SetStabHigh();
}

void AOedivXuejCharacter::ServerSetStabHigh_Implementation()
{
	MultiCastSetStabHigh();
}

bool AOedivXuejCharacter::ServerSetStabHigh_Validate()
{
	return true;
}

void AOedivXuejCharacter::OnStabHigh()
{
	if (Energy >= 0.10)
	{
		float ws = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
		if (canMove() && ws > 500) {
			Energy -= 0.10;
			UpdateEnergyPercent();
			Info = "";
			if (HasAuthority())
			{
				MultiCastSetStabHigh();
			}
			else
			{
				ServerSetStabHigh();
			}
		}
	}
	else
		Info = "Not Enought Energy !";
}

void AOedivXuejCharacter::SetStabHigh()
{
	// try and play a firing animation if specified
	if (StabHighAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(StabHighAnimation, 1.f);
			canStabHigh = false;
		}
	}
}

//Attack Jump Stab
void AOedivXuejCharacter::MultiCastSetStabJump_Implementation()
{
	SetStabJump();
}

void AOedivXuejCharacter::ServerSetStabJump_Implementation()
{
	MultiCastSetStabJump();
}

bool AOedivXuejCharacter::ServerSetStabJump_Validate()
{
	return true;
}

void AOedivXuejCharacter::OnStabJump()
{
	if (Energy >= 0.10)
	{
		float ws = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
		if (canMove() && ws > 500) {
			Energy -= 0.10;
			UpdateEnergyPercent();
			Info = "";
			if (HasAuthority())
			{
				MultiCastSetStabJump();
			}
			else
			{
				ServerSetStabJump();
			}
		}
	}
	else
		Info = "Not Enought Energy !";
}

void AOedivXuejCharacter::SetStabJump()
{
	// try and play a firing animation if specified
	if (StabJumpAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			Jump();
			AnimInstance->Montage_Play(StabJumpAnimation, 1.f);
			canStabJump = false;
		}
	}
}

	void AOedivXuejCharacter::SetEnergy(float EnergyChange)
	{
		float NewEnergy = Energy + EnergyChange;
		if (NewEnergy >= 0.0 && NewEnergy <= 1.0)
			Energy = NewEnergy;
		else if (NewEnergy < 0.0)
			Energy = 0.0;
		else
			Energy = 1.0;
	}

	void AOedivXuejCharacter::SetHealth(float HealthChange)
	{
		float NewHealth = Health + HealthChange;
		if (NewHealth >= 0.0 && NewHealth <= 1.0)
			Health = NewHealth;
		else if (NewHealth < 0.0)
			Health = 0.0;
		else
			Health = 1.0;
	}

	/* **************** TEST ***************************** */

	void AOedivXuejCharacter::TestStats()
	{
		TestHealth();
		TestEnergy();
	}

	void AOedivXuejCharacter::TestHealth()
	{
		if (Health != 1.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Health start value : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Health start value : 1.0 OK")));

		SetHealth(2.0);
		if (Health != 1.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Health max value : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Health max value : 1.0 OK")));

		SetHealth(-2.0);
		if (Health != 0.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Health min value : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Health min value : 0.0 OK")));

		SetHealth(1.0);
	}

	void AOedivXuejCharacter::TestEnergy()
	{
		if (Energy != 1.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Energy start value : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Energy start value : 1.0 OK")));

		SetEnergy(2.0);
		if (Energy != 1.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Energy max value : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Energy max value : 1.0 OK")));

		SetEnergy(-2.0);
		if (Energy != 0.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Energy min value : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Energy min value : 0.0 OK")));

		SetEnergy(1.0);
		JumpRoll();
		if (Energy < 0.8 || Energy > 0.85)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Energy after jump : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Energy after jump : 0.8 OK")));

		SetEnergy(-1.0);
		JumpRoll();
		if (GetCharacterMovement()->Velocity.Z != 0 || Energy < 0.0)
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Red, FString::Printf(TEXT("TEST : Jump when no energy : FAIL")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 120.0f, FColor::Green, FString::Printf(TEXT("TEST : Jump when no energy : OK")));
	}
