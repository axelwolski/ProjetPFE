// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Boss3.generated.h"

UCLASS()
class OEDIVXUEJ_API ABoss3 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool StartFight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool HasHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool TakeHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool BeginAnimationAttack = false;

	UAnimInstance* AnimInstance;

	//Animation Hide 
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastHide();
	void MultiCastHide_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerHide();
	void ServerHide_Implementation();
	bool ServerHide_Validate();

	void OnHide();
	void SetHide();
	bool Hide = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* HideAnimation;

	//Animation Attack Base 
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastAttackBase();
	void MultiCastAttackBase_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttackBase();
	void ServerAttackBase_Implementation();
	bool ServerAttackBase_Validate();

	void OnAttackBase();
	void SetAttackBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* AttackBaseAnimation;

	//Animation Charge 
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastCharge();
	void MultiCastCharge_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerCharge();
	void ServerCharge_Implementation();
	bool ServerCharge_Validate();

	void OnCharge();
	void SetCharge();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		bool CanCharge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* ChargeAnimation;


	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
};
