// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationCharacter.h"

void UAnimationCharacter::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();
}

//Tick
void UAnimationCharacter::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn)
	{
		return;
	}
}

