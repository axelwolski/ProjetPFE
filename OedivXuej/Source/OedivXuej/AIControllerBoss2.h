// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss2.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "OedivXuejCharacter.h"
#include "AIControllerBoss2.generated.h"

/**
 * 
 */
UCLASS()
class OEDIVXUEJ_API AAIControllerBoss2 : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBoss2(const FObjectInitializer& ObjectInitializer);

		UPROPERTY(transient)
			UBlackboardComponent* BlackboardComp;

		UPROPERTY(transient)
			UBehaviorTreeComponent* BehaviorComp;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = Info)
			TArray<AOedivXuejCharacter*> TabCharacter;

		virtual void Possess(APawn *InPawn);

		void SetEnemy(APawn* InPawn);

		UFUNCTION(BlueprintCallable, Category = Behavior)
			void SearchForEnemy();

		virtual void Tick(float DeltaSeconds) override;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			bool RecupActor = true;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			FVector BossPos;

		int MoveToCharacter();
		void Attack(int finishMove);

		bool FinishAttack = false;

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

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, Replicated)
			class USkeletalMeshComponent* MeshBoss;

		void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

protected:
	uint8 EnemyKeyID;
	uint8 EnemyLocationID;
};
