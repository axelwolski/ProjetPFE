// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Boss2.generated.h"

UCLASS()
class OEDIVXUEJ_API ABoss2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Stats)
		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool StartFight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool HasHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		int GetOut = 0;

	void SetGetOut(int NewGetOut);
	int GetGetOut();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool BeginAnimationAttack = false;

	UAnimInstance* AnimInstance;

	//Animation Attack Basic 
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastAttackBasic();
	void MultiCastAttackBasic_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttackBasic();
	void ServerAttackBasic_Implementation();
	bool ServerAttackBasic_Validate();

	void OnAttackBasic();
	void SetAttackBasic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* AttackAnimationBasic;

	//Animation Attack Slash Right
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastAttackSlashRight();
	void MultiCastAttackSlashRight_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttackSlashRight();
	void ServerAttackSlashRight_Implementation();
	bool ServerAttackSlashRight_Validate();

	void OnAttackSlashRight();
	void SetAttackSlashRight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* AttackAnimationSlashRight;

	//Animation Attack Slash Left
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastAttackSlashLeft();
	void MultiCastAttackSlashLeft_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttackSlashLeft();
	void ServerAttackSlashLeft_Implementation();
	bool ServerAttackSlashLeft_Validate();

	void OnAttackSlashLeft();
	void SetAttackSlashLeft();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* AttackAnimationSlashLeft;

	//Animation Attack Slash Left
	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastAttackRage();
	void MultiCastAttackRage_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttackRage();
	void ServerAttackRage_Implementation();
	bool ServerAttackRage_Validate();

	void OnAttackRage();
	void SetAttackRage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* AttackAnimationRage;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
