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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool StartFight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		bool HasHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
		FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool BeginAnimationAttack = false;

	UAnimInstance* AnimInstance;

	UFUNCTION(NetMulticast, Unreliable)
		void MultiCastAttack();
	void MultiCastAttack_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttack();
	void ServerAttack_Implementation();
	bool ServerAttack_Validate();

	void OnAttack();
	void SetAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
		class UAnimMontage* AttackAnimation;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
