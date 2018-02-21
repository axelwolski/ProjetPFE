// Fill out your copyright notice in the Description page of Project Settings.

#include "TestLevel.h"

#include "Components/ActorComponent.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "stdlib.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/BlockingVolume.h"
#include "Runtime/Engine/Classes/Engine/DirectionalLight.h"
#include "Runtime/Engine/Classes/Engine/ExponentialHeightFog.h"
#include "Runtime/Engine/Classes/Engine/SkyLight.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Level.h"


// Sets default values
UTestLevel::UTestLevel()
{
	nbTest = 0;
}

// Called when the game starts or when spawned
void UTestLevel::TestAll()
{
	nbTest = 0;

	objectList.Add(Actors());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of Actors = ")) + FString::FromInt(objectList[nbTest++]));
	objectList.Add(StaticMeshActors());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of StaticMeshActors = ")) + FString::FromInt(objectList[nbTest++]));
	objectList.Add(BlockingVolumes());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of BlockingVolumes = ")) + FString::FromInt(objectList[nbTest++]));
	objectList.Add(DirectionalLights());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of DirectionalLights = ")) + FString::FromInt(objectList[nbTest++]));
	objectList.Add(ExponentialHeightFogs());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of ExponentialHeightFogs = ")) + FString::FromInt(objectList[nbTest++]));
	objectList.Add(SkyLights());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of SkyLights = ")) + FString::FromInt(objectList[nbTest++]));
	objectList.Add(PlayerStarts());
	nbTest++;
	//GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Number of PlayerStarts = ")) + FString::FromInt(objectList[nbTest]));

}

bool UTestLevel::TestLevelArena(int count)
{
	TestAll();
	TArray<int> objectList = GetObjectList();
	TArray<int> objectListTmp;
	int nbTest = GetNbTest();

	for (int j = 0; j < count; j++)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Arena_V1", true, "listen");
		TestAll();
		objectListTmp = GetObjectList();
		for (int i = 0; i < nbTest; ++i)
		{
			if (objectList[i] != objectListTmp[i])
			{
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::FromInt(objectList[i]) + FString::Printf(TEXT("!=")) + FString::FromInt(objectListTmp[i]));
				return EXIT_FAILURE;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("Test pass : ")) + FString::FromInt(j));
		objectListTmp.Empty();
	}
	objectList.Empty();
	return EXIT_SUCCESS;
}

void UTestLevel::TestArena(int count)
{
	if (TestLevelArena(count))
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, FString::Printf(TEXT("Test finish - fail")));
	else
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Green, FString::Printf(TEXT("Test finish - ok")));
}


int UTestLevel::Actors()
{
	int sizeActors = 0;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}

int UTestLevel::BlockingVolumes()
{
	int sizeActors = 0;
	for (TActorIterator<ABlockingVolume> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}

int UTestLevel::StaticMeshActors()
{
	int sizeActors = 0;
	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}

int UTestLevel::DirectionalLights()
{
	int sizeActors = 0;
	for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}

int UTestLevel::ExponentialHeightFogs()
{
	int sizeActors = 0;
	for (TActorIterator<AExponentialHeightFog> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}

int UTestLevel::SkyLights()
{
	int sizeActors = 0;
	for (TActorIterator<ASkyLight> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}

int UTestLevel::PlayerStarts()
{
	int sizeActors = 0;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		sizeActors++;
	}
	return sizeActors;
}
