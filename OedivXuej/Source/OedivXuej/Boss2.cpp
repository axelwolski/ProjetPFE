// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss2.h"
#include <stdlib.h>
#include <time.h>
#include "Runtime/Engine/Classes/Engine/Engine.h"

// Sets default values
ABoss2::ABoss2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 1.0f;
	Name = "Forge Master";
	HasHit = false;
}

// Called when the game starts or when spawned
void ABoss2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsAttacking)
	{
		if (GetOut > 100) 
		{
			//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::White, FString("\nAOE Rage begin\n"));
			OnAttackRage();
		}
		else
		{
			/*
			CptAttack++;
			if (CptAttack == 100)
			{
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Attack 3 : ") + FString::FromInt(Cpt2));
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Attack 2 : ") + FString::FromInt(Cpt1));
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Attack 1 : ") + FString::FromInt(Cpt0));
			}*/
			int R = FMath::RandRange(0, 2);
			//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Random number is : ") + FString::FromInt(R));
			if (R == 0)
			{
				OnAttackBasic();
				/*
				Cpt0++;
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Attack select is Attack1"));
				*/
			}
			else if (R == 1)
			{
				OnAttackSlashLeft();
				/*
				Cpt1++;
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Attack select is Attack2"));
				*/
			}
			else
			{
				OnAttackSlashRight();
				/*
				Cpt2++;
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString("Attack select is Attack3"));
				*/
			}
		}
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(AttackAnimationBasic) && !AnimInstance->Montage_IsPlaying(AttackAnimationSlashRight) && !AnimInstance->Montage_IsPlaying(AttackAnimationSlashLeft) && !AnimInstance->Montage_IsPlaying(AttackAnimationRage))
	{
		BeginAnimationAttack = false;

	}
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(BeginAnimationAttack));
}

void ABoss2::SetGetOut(int NewGetOut)
{
	GetOut = NewGetOut;
}

int ABoss2::GetGetOut()
{
	return GetOut;
}

// Called to bind functionality to input
void ABoss2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Attack Basic
void ABoss2::MultiCastAttackBasic_Implementation()
{
	SetAttackBasic();
}

void ABoss2::ServerAttackBasic_Implementation()
{
	MultiCastAttackBasic();
}

bool ABoss2::ServerAttackBasic_Validate()
{
	return true;
}

void ABoss2::OnAttackBasic()
{

	if (HasAuthority())
	{
		MultiCastAttackBasic();
	}
	else
	{
		ServerAttackBasic();
	}
}

void ABoss2::SetAttackBasic()
{
	// try and play a firing animation if specified
	if (AttackAnimationBasic != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimationBasic, 1.f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

//Attack Slahs Right
void ABoss2::MultiCastAttackSlashRight_Implementation()
{
	SetAttackSlashRight();
}

void ABoss2::ServerAttackSlashRight_Implementation()
{
	MultiCastAttackSlashRight();
}

bool ABoss2::ServerAttackSlashRight_Validate()
{
	return true;
}

void ABoss2::OnAttackSlashRight()
{

	if (HasAuthority())
	{
		MultiCastAttackSlashRight();
	}
	else
	{
		ServerAttackSlashRight();
	}
}

void ABoss2::SetAttackSlashRight()
{
	// try and play a firing animation if specified
	if (AttackAnimationSlashRight != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimationSlashRight, 1.f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

// Attack Slash Left
void ABoss2::MultiCastAttackSlashLeft_Implementation()
{
	SetAttackSlashLeft();
}

void ABoss2::ServerAttackSlashLeft_Implementation()
{
	MultiCastAttackSlashLeft();
}

bool ABoss2::ServerAttackSlashLeft_Validate()
{
	return true;
}

void ABoss2::OnAttackSlashLeft()
{

	if (HasAuthority())
	{
		MultiCastAttackSlashLeft();
	}
	else
	{
		ServerAttackSlashLeft();
	}
}

void ABoss2::SetAttackSlashLeft()
{
	// try and play a firing animation if specified
	if (AttackAnimationSlashLeft != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimationSlashLeft, 1.f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

// Attack Rage
void ABoss2::MultiCastAttackRage_Implementation()
{
	SetAttackRage();
}

void ABoss2::ServerAttackRage_Implementation()
{
	MultiCastAttackRage();
}

bool ABoss2::ServerAttackRage_Validate()
{
	return true;
}

void ABoss2::OnAttackRage()
{

	if (HasAuthority())
	{
		MultiCastAttackRage();
	}
	else
	{
		ServerAttackRage();
	}
}

void ABoss2::SetAttackRage()
{
	// try and play a firing animation if specified
	if (AttackAnimationRage != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimationRage, 1.f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

void ABoss2::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


