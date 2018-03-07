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

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			bool IsAttacking;

		UFUNCTION(BlueprintCallable, Category = Behavior)
			int MoveToCharacter();
		void Attack(int finishMove);

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			bool BeginAttack = false;

protected:
	uint8 EnemyKeyID;
	uint8 EnemyLocationID;
};
