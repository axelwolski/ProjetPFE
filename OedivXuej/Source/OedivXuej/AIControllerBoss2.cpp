
//#include "SideScrollerConcept.h"
#include "AIControllerBoss2.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
//#include "SideScrollerConceptCharacter.h"


AAIControllerBoss2::AAIControllerBoss2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}

void AAIControllerBoss2::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
}

void AAIControllerBoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int AAIControllerBoss2::MoveToCharacter(AOedivXuejCharacter* AgroCheck)
{
	return MoveToLocation(AgroCheck->GetActorLocation(), 500.f);
}

AOedivXuejCharacter* AAIControllerBoss2::AgroCheck()
{
	int Index = 0;
	for (int i = 0; i < TabCharacter.Num(); i++)
	{
		if (TabCharacter[i]->Health > 0)
		{
			Index = i;
			break;
		}
	}
	float DistMin = FVector::Distance(TabCharacter[Index]->GetActorLocation(), Boss2->GetActorLocation());
	TargetToFollow = TabCharacter[Index];
	for (int i = 1; i < TabCharacter.Num(); i++)
	{
		float TmpDist = FVector::Distance(TabCharacter[i]->GetActorLocation(), Boss2->GetActorLocation());
		if (DistMin > TmpDist && TabCharacter[i]->Health > 0)
		{
			DistMin = TmpDist;
			Index = i;
			TargetToFollow = TabCharacter[i];
		}
	}
	return TargetToFollow;
}

void AAIControllerBoss2::SaveProba() 
{
	FVector tmp = FVector(Boss2->GetActorForwardVector().X - Boss2->GetActorLocation().X, Boss2->GetActorForwardVector().Y - Boss2->GetActorLocation().Y, Boss2->GetActorForwardVector().Z - Boss2->GetActorLocation().Z);
	float angle = acos(Boss2->GetActorForwardVector().DotProduct(Boss2->GetActorForwardVector(), tmp));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(angle));
}
