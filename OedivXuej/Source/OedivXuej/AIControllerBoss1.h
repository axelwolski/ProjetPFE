// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss1.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "OedivXuejCharacter.h"
#include "AIControllerBoss1.generated.h"

/**
 * 
 */
UCLASS()
class OEDIVXUEJ_API AAIControllerBoss1 : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = Info)
		TArray<AOedivXuejCharacter*> TabCharacter;

	virtual void Possess(APawn *InPawn);

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool RecupActor = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		AOedivXuejCharacter* TargetToFollow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		ABoss1* Boss1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool BeginAttack = false;	
	
};
