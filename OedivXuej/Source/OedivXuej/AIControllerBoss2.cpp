
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
		if (TabCharacter[i]->HealthCharacter > 0)
		{
			Index = i;
			break;
		}
	}
	float DistMin = FVector::Distance(TabCharacter[Index]->GetActorLocation(), Boss2->GetActorLocation());
	/*
	float TmpHealth = TabCharacter[0]->HealthCharacter * 100;
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Player") + FString::FromInt(0) + FString(", distance : ") + FString::FromInt(DistMin) + FString(", health : ") + FString::FromInt(TmpHealth) + FString("\n"));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("GetOut value : ") + FString::FromInt(Boss2->GetGetOut()));
	*/
	TargetToFollow = TabCharacter[Index];
	for (int i = 1; i < TabCharacter.Num(); i++)
	{
		float TmpDist = FVector::Distance(TabCharacter[i]->GetActorLocation(), Boss2->GetActorLocation());
		/*
		TmpHealth = TabCharacter[i]->HealthCharacter * 100;
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Player") + FString::FromInt(i) + FString(", distance : ") + FString::FromInt(TmpDist) + FString(", health : ") + FString::FromInt(TmpHealth));
		*/
		if (DistMin > TmpDist && TabCharacter[i]->HealthCharacter > 0)
		{
			DistMin = TmpDist;
			Index = i;
			TargetToFollow = TabCharacter[i];
		}
	}
	
	/*
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Distance between boss/players :"));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("Boss is attacking : ") + FString::FromInt(Boss2->BeginAnimationAttack));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("\n"));
	
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Boss TargetToFollow : ") + FString("Player") + FString::FromInt(Index));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("\n"));
	*/
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

		
		/*GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("FrontLeft: ") + FString::FromInt(ProbaHautGauche));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("FrontRight: ") + FString::FromInt(ProbaHautDroite));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("BackRight: ") + FString::FromInt(ProbaBasDroite));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("BackLeft: ") + FString::FromInt(ProbaBasGauche));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Probability before attack:"));
		*/

		if (Angle > 180.0f)
		{
			Angle -= 360.0f;
		}
		else if (Angle < -180.0f)
		{
			Angle += 360.0f;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::White, FString("\n") + FString("Angle: ") + FString::FromInt(Angle) + FString(" Degree \n"));

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
	
	/*GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FrontLeft: ") + FString::FromInt(ProbaHautGauche));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FrontRight: ") + FString::FromInt(ProbaHautDroite));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("BackRight: ") + FString::FromInt(ProbaBasDroite));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("BackLeft: ") + FString::FromInt(ProbaBasGauche));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("\n \n \n") + FString("Probability after attack:"));
	*/

}


FVector AAIControllerBoss2::GetDirectionProba()
{

	float HD = (float)ProbaHautDroite / (float)SommeAttaque * 100.f;
	float BD = (float)ProbaBasDroite / (float)SommeAttaque * 100.f;
	float BG = (float)ProbaBasGauche / (float)SommeAttaque * 100.f;
	float HG = (float)ProbaHautGauche / (float)SommeAttaque * 100.f;

	/*
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FrontLeft: ") + FString::FromInt(HG));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FrontRight: ") + FString::FromInt(HD));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("BackRight: ") + FString::FromInt(BD));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("BackLeft: ") + FString::FromInt(BG));
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("Probability Direction:"));
	*/

	int MaxR = HD + BD + BG + HG;
	int R = FMath::RandRange(1, MaxR);
	Tmp = FVector(0.0, 0.0, 0.0);
	FRotator Rot = Boss2->GetActorForwardVector().Rotation();

	/*
	float BeforeYaw = Rot.Yaw;
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::White, FString("Random: ") + FString::FromInt(R));
	FString dir;
	*/

	if (R <= HD)
	{
		Rot.Yaw += 45.f;
		/*dir = "FrontRight";
		AvDroite++;
		*/
	}
	else if (R > HD && R <= (HD + BD))
	{
		Rot.Yaw += 135.f;
		/*dir = "BackRight";
		DerDroite++;
		*/
	}
	else if (R > (HD + BD) && R <= (HD + BD + BG))
	{
		Rot.Yaw += 225.f;
		/*dir = "BackLeft";
		DerGauche++;
		*/
	}
	else
	{
		Rot.Yaw += 315.f;
		/*dir = "FrontLeft";
		AvGauche++;
		*/
	}

	/*tt++;
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::White, FString("\n") + FString("Direction attack: ") + FString::FromInt(Rot.Yaw - BeforeYaw) + FString(" degree which corresponds to ") + dir + FString("\n"));
	if (tt == 100)
	{
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FrontLeft: ") + FString::FromInt(AvGauche));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FrontRight: ") + FString::FromInt(AvDroite));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("BackRight: ") + FString::FromInt(DerDroite));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("BackLeft: ") + FString::FromInt(DerGauche));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString("FeedBack probability :"));
	}*/
	return FVector(Rot.Vector().X, Rot.Vector().Y, 0.f); // FVector(ReturnX, ReturnY, 0.f);
}