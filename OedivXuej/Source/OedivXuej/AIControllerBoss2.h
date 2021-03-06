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

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool RecupActor = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		AOedivXuejCharacter* TargetToFollow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		ABoss2* Boss2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool IsAttacking;

	UFUNCTION(BlueprintCallable, Category = Behavior)
		int MoveToCharacter(AOedivXuejCharacter* AgroCheck);

	UFUNCTION(BlueprintCallable, Category = Behavior)
		void SaveProba();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float Angle = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		int ProbaHautDroite = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		int ProbaBasGauche = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		int ProbaHautGauche = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		int ProbaBasDroite = 0;
	int SommeAttaque = 1;

	//Variable test proba
	int AvGauche = 0;
	int AvDroite = 0;
	int DerGauche = 0;
	int DerDroite = 0;
	int tt = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		FVector Tmp;

	UFUNCTION(BlueprintCallable, Category = Behavior)
		FVector GetDirectionProba();

	UFUNCTION(BlueprintCallable, Category = Behavior)
		AOedivXuejCharacter* AgroCheck();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool BeginAttack = false;
};
