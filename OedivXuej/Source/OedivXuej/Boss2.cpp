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
		OnAttack();
	}
	if (AnimInstance != NULL && !AnimInstance->Montage_IsPlaying(AttackAnimation))
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

void ABoss2::MultiCastAttack_Implementation()
{
	SetAttack();
}

void ABoss2::ServerAttack_Implementation()
{
	MultiCastAttack();
}

bool ABoss2::ServerAttack_Validate()
{
	return true;
}

void ABoss2::OnAttack()
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

void ABoss2::SetAttack()
{
	// try and play a firing animation if specified
	if (AttackAnimation != NULL)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		// Get the animation object for the arms mesh
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimation, 1.f);
			BeginAnimationAttack = true;
			IsAttacking = false;
		}
	}
}

void ABoss2::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
