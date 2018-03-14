
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
	if (TargetToFollow != NULL && Boss2 != NULL)
	{
		FVector2D tmp = FVector2D(Boss2->GetActorLocation().X - TargetToFollow->GetActorLocation().X, Boss2->GetActorLocation().Y - TargetToFollow->GetActorLocation().Y);
		FVector2D Forward2D = FVector2D(Boss2->GetActorForwardVector().X, Boss2->GetActorForwardVector().Y);
		float Ang1 = FMath::Atan2(tmp.X, tmp.Y);
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

		/*if (Angle <= 180.f && Angle > 90.f)
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
		}*/
		//SommeAttaque++;
	}
}


FVector AAIControllerBoss2::GetDirectionProba()
{

	float hd = ProbaHautDroite / SommeAttaque * 100;
	float bd = ProbaBasDroite / SommeAttaque * 100;
	float bg = ProbaBasGauche / SommeAttaque * 100;
	float hg = ProbaHautGauche / SommeAttaque * 100;

	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(SommeAttaque));
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::FromInt(hd));
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(bg));

	int r = FMath::RandRange(1, 100);
	tmp = FVector(0.0, 0.0, 0.0);
	float returnX;
	float returnY;

	if (r <= hd)
	{
		tmp = Boss2->GetActorForwardVector().RotateAngleAxis(45.f, Boss2->GetActorForwardVector());
	}
	else if (r > hd && r <= (hd + bd))
	{
		tmp = Boss2->GetActorForwardVector().RotateAngleAxis(135.f, Boss2->GetActorForwardVector());
	}
	else if (r > (hd + bd) && r <= (hd + bd + bg))
	{
		tmp = Boss2->GetActorForwardVector().RotateAngleAxis(225.0f, Boss2->GetActorForwardVector());

		float Ang1 = FMath::Atan2(tmp.X, tmp.Y);
		float Ang2 = FMath::Atan2(Boss2->GetActorForwardVector().X, Boss2->GetActorForwardVector().Y);
		Angle = FMath::RadiansToDegrees(Ang1 - Ang2);

		if (Angle > 180.0f)
		{
			Angle -= 360.0f;
		}
		else if (Angle < -180.0f)
		{
			Angle += 360.0f;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(Angle));
	}
	else
	{
		tmp = Boss2->GetActorForwardVector().RotateAngleAxis(315.f, Boss2->GetActorForwardVector());
	}

	if (tmp.X > 0)
	{
		returnX = 10;
	}
	else
	{
		returnX = -10;
	}
	if (tmp.Y > 0)
	{
		returnY = 10;
	}
	else
	{
		returnY = -10;
	}
	return FVector(returnX, returnY, 0.f);
}