// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss3.h"
#include <stdlib.h>
#include <time.h>
#include "Runtime/Engine/Classes/Engine/Engine.h"


// Sets default values
ABoss3::ABoss3()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 1.0f;
	Name = "Task Master";
	HasHit = false;
}

// Called when the game starts or when spawned
void ABoss3::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoss3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health < 0.5 && !Hide)
	{
		OnHide();
		Hide = true;
	}
	if (IsAttacking)
	{
		OnAttackBase();
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(HideAnimation))
	{
		BeginAnimationAttack = false;
	}
}

// Hide
void ABoss3::MultiCastHide_Implementation()
{
	SetHide();
}

void ABoss3::ServerHide_Implementation()
{
	MultiCastHide();
}

bool ABoss3::ServerHide_Validate()
{
	return true;
}

void ABoss3::OnHide()
{

	if (HasAuthority())
	{
		MultiCastHide();
	}
	else
	{
		ServerHide();
	}
}

void ABoss3::SetHide()
{
	// try and play a firing animation if specified
	if (HideAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(HideAnimation, 0.7f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

// Attack Base
void ABoss3::MultiCastAttackBase_Implementation()
{
	SetAttackBase();
}

void ABoss3::ServerAttackBase_Implementation()
{
	MultiCastAttackBase();
}

bool ABoss3::ServerAttackBase_Validate()
{
	return true;
}

void ABoss3::OnAttackBase()
{

	if (HasAuthority())
	{
		MultiCastAttackBase();
	}
	else
	{
		ServerAttackBase();
	}
}

void ABoss3::SetAttackBase()
{
	// try and play a firing animation if specified
	if (HideAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackBaseAnimation, 1.0f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

// Charge
void ABoss3::MultiCastCharge_Implementation()
{
	SetCharge();
}

void ABoss3::ServerCharge_Implementation()
{
	MultiCastCharge();
}

bool ABoss3::ServerCharge_Validate()
{
	return true;
}

void ABoss3::OnCharge()
{

	if (HasAuthority())
	{
		MultiCastCharge();
	}
	else
	{
		ServerCharge();
	}
}

void ABoss3::SetCharge()
{
	// try and play a firing animation if specified
	if (HideAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(ChargeAnimation, 1.0f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

void ABoss3::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

