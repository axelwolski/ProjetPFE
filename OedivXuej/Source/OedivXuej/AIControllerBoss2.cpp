
//#include "SideScrollerConcept.h"
#include "AIControllerBoss2.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include <stdlib.h>
#include <time.h>
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
	bool Rage = false;
	for (int i = 0; i < TabCharacter.Num(); i++)
	{
		float TmpDist = FVector::Distance(TabCharacter[i]->GetActorLocation(), Boss2->GetActorLocation());
		if (TmpDist < 500)
		{
			int Tmp = Boss2->GetGetOut() + 1;
			Boss2->SetGetOut(Tmp);
			Rage = true;
		}
	}
	if (!Rage && Boss2 != NULL)
	{
		int Tmp2 = 0;
		Boss2->SetGetOut(0);
	}
}

int AAIControllerBoss2::MoveToCharacter(AOedivXuejCharacter* AgroCheck)
{
	if (Boss2->StartFight)
	{
		return MoveToLocation(AgroCheck->GetActorLocation(), 500.f); // 500 -> Distance entre le joueur et le boss pour que l'arme touche
	}
	return -1;
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
	if (TargetToFollow != NULL && Boss2 != NULL)
	{
		FVector2D Tmp = FVector2D(Boss2->GetActorLocation().X - TargetToFollow->GetActorLocation().X, Boss2->GetActorLocation().Y - TargetToFollow->GetActorLocation().Y);
		FVector2D Forward2D = FVector2D(Boss2->GetActorForwardVector().X, Boss2->GetActorForwardVector().Y);
		float Ang1 = FMath::Atan2(Tmp.X, Tmp.Y);
		float Ang2 = FMath::Atan2(Forward2D.X, Forward2D.Y);
		Angle = FMath::RadiansToDegrees(Ang1 - Ang2);

		if (Angle > 180.0f)
		{
			Angle -= 360.0f;
		}
		else if (Angle < -180.0f)
		{
			Angle += 360.0f;
		}

		if (Angle <= 180.f && Angle > 90.f)
		{
		ProbaHautDroite++;
		}
		else if(Angle <= 90.f && Angle > 0.f)
		{
		ProbaBasDroite++;
		}
		else if(Angle <= 0.f && Angle > -90.f)
		{
		ProbaBasGauche++;
		}
		else if(Angle <= -90.f && Angle > -180.f)
		{
		ProbaHautGauche++;
		}
		SommeAttaque++;
	}
}


FVector AAIControllerBoss2::GetDirectionProba()
{

	float HD = ProbaHautDroite / SommeAttaque * 100;
	float BD = ProbaBasDroite / SommeAttaque * 100;
	float BG = ProbaBasGauche / SommeAttaque * 100;
	float HG = ProbaHautGauche / SommeAttaque * 100;

	int R = FMath::RandRange(1, 100);
	Tmp = FVector(0.0, 0.0, 0.0);
	float ReturnX;
	float ReturnY;
	FRotator Rot = Boss2->GetActorForwardVector().Rotation();
	if (R <= HD)
	{
		Rot.Yaw += 45.f;
	}
	else if (R > HD && R <= (HD + BD))
	{
		Rot.Yaw += 135.f;
	}
	else if (R > (HD + BD) && R <= (HD + BD + BG))
	{
		Rot.Yaw += 225.f;
	}
	else
	{
		Rot.Yaw += 315.f;
	}
	Tmp = Rot.Vector();

	if (Tmp.X > 0)
	{
		ReturnX = 10;
	}
	else
	{
		ReturnX = -10;
	}
	if (Tmp.Y > 0)
	{
		ReturnY = 10;
	}
	else
	{
		ReturnY = -10;
	}
	return FVector(ReturnX, ReturnY, 0.f);
}