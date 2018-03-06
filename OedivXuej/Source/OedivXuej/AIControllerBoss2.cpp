
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
	
	/*int sizeActors = 0;
	for (TActorIterator<AOedivXuejCharacter> It(GetWorld()); It; ++It)
	{
		sizeActors++;
		tabCharacter.Add(*It);
	}
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(tabCharacter.Max()));*/
}

void AAIControllerBoss2::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	//MoveToLocation(tabCharacter[0]->GetActorLocation(), 100.f);
}

void AAIControllerBoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!RecupActor) 
	{
		int finishMove = MoveToCharacter();
		Attack(finishMove);
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(AttackAnimation)) {
		FinishAttack = false;
	}
}

int AAIControllerBoss2::MoveToCharacter() 
{
	if (!FinishAttack)
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
		float DistMin = FVector::Distance(TabCharacter[Index]->GetActorLocation(), BossPos);
		for (int i = 1; i < TabCharacter.Num(); i++)
		{
			float TmpDist = FVector::Distance(TabCharacter[i]->GetActorLocation(), BossPos);
			if (DistMin > TmpDist && TabCharacter[i]->Health > 0)
			{
				DistMin = TmpDist;
				Index = i;
			}
		}
		return MoveToLocation(TabCharacter[Index]->GetActorLocation(), 500.f);
	}
	return -1;
}

void AAIControllerBoss2::Attack(int FinishMove) 
{
	if (FinishMove == 1) 
	{
		FinishAttack = true;
		OnAttack();
	}
}

void AAIControllerBoss2::MultiCastAttack_Implementation()
{
	SetAttack();
}

void AAIControllerBoss2::ServerAttack_Implementation()
{
	MultiCastAttack();
}

bool AAIControllerBoss2::ServerAttack_Validate()
{
	return true;
}

void AAIControllerBoss2::OnAttack()
{
	
	if (HasAuthority())
	{
		MultiCastAttack();
	}
	else
	{
		ServerAttack();
	}
}

void AAIControllerBoss2::SetAttack()
{
	// try and play a firing animation if specified
	if (AttackAnimation != NULL)
	{
		AnimInstance = MeshBoss->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimation, 1.f);
		}
	}
}

void AAIControllerBoss2::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

