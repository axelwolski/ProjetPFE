// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimationCharacter.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "OedivXuejCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);

UCLASS(config=Game)
class AOedivXuejCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AOedivXuejCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		float Energy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		FString Info;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		FString HealthPercent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		FString EnergyPercent;

	UFUNCTION(BlueprintCallable, Category = Stats)
		void RefillEnergy();


protected:

	void UpdateEnergyPercent();
	void UpdateHealthPercent();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void JumpRoll();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void Tick(float DeltaSeconds);

	bool firstRight;
	FVector precRight;

	bool firstForward;
	FVector precForward;

	UAnimInstance* AnimInstance;
	bool canMove();

	//Animation Roll
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastSetRoll();
	void MultiCastSetRoll_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetRoll();
	void ServerSetRoll_Implementation();
	bool ServerSetRoll_Validate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		class UAnimMontage* RollAnimation;

	void OnRoll();
	void SetRolling();
	bool canRoll;

	//Animation Stab
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastSetStab(UAnimMontage* animStab);
	void MultiCastSetStab_Implementation(UAnimMontage* animStab);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetStab(UAnimMontage* animStab);
	void ServerSetStab_Implementation(UAnimMontage* animStab);
	bool ServerSetStab_Validate(UAnimMontage* animStab);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		class UAnimMontage* StabAnimation;

	void OnStab();
	void SetStab(UAnimMontage* animStab);
	bool canStab;
	/*int numberStab;
	bool continueStab;
	bool moveStab;*/

	//Animation High Stab
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastSetStabHigh();
	void MultiCastSetStabHigh_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetStabHigh();
	void ServerSetStabHigh_Implementation();
	bool ServerSetStabHigh_Validate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		class UAnimMontage* StabHighAnimation;

	void OnStabHigh();
	void SetStabHigh();
	bool canStabHigh;

	//Animation Jump Stab
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastSetStabJump();
	void MultiCastSetStabJump_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetStabJump();
	void ServerSetStabJump_Implementation();
	bool ServerSetStabJump_Validate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		class UAnimMontage* StabJumpAnimation;

	void OnStabJump();
	void SetStabJump();
	bool canStabJump;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

