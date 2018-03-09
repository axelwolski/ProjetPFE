// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss2.h"
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
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(IsAttacking));
	if (IsAttacking)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(IsAttacking));
		OnAttackBasic();
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(AttackAnimationBasic) && !AnimInstance->Montage_IsPlaying(AttackAnimationSlashRight) && !AnimInstance->Montage_IsPlaying(AttackAnimationSlashLeft))
	{
		BeginAnimationAttack = false;
	} 
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(BeginAnimationAttack));
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

// Slahs Right
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

// Attack Basic
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

void ABoss2::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
