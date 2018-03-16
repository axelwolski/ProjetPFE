// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss3.h"


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

}

// Called to bind functionality to input
void ABoss3::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

