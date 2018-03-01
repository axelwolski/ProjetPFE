
//#include "SideScrollerConcept.h"
#include "AIControllerBoss2.h"
//#include "SideScrollerConceptCharacter.h"


AAIControllerBoss2::AAIControllerBoss2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}

void AAIControllerBoss2::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	/*ACorruptBot* Bot = Cast<ACorruptBot>(InPawn);
	if (Bot && Bot->BotBehavior)
	{
		BlackboardComp->InitializeBlackboard(Bot->BotBehavior->BlackboardAsset);

		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		EnemyLocationID = BlackboardComp->GetKeyID("Destination");

		BehaviorComp->StartTree(Bot->BotBehavior);
	}
	*/
}

void AAIControllerBoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToLocation(ActorPos, 100.0f);

}

void AAIControllerBoss2::SearchForEnemy()
{
	/*APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	ASideScrollerConceptCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		ASideScrollerConceptCharacter* TestPawn = Cast<ASideScrollerConceptCharacter>(*It);
		if (TestPawn)
		{
			const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}
	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}*/
}

void AAIControllerBoss2::SetEnemy(class APawn* InPawn)
{
	/*BlackboardComp->SetValueAsObject(EnemyKeyID, InPawn);
	BlackboardComp->SetValueAsVector(EnemyLocationID, InPawn->GetActorLocation());
	SetFocus(InPawn);*/
}
/*
class ASideScrollerConceptCharacter* AAIControllerBoss2::GetEnemy() const
{
	if (BlackboardComp)
	{
		return Cast<ASideScrollerConceptCharacter>(BlackboardComp->GetValueAsObject(EnemyKeyID));
	}
	
	return NULL;
}*/

