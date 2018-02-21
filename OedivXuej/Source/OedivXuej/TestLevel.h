// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OedivXuej.h"
#include "Engine/GameInstance.h"
#include "TestLevel.generated.h"

UCLASS()
class OEDIVXUEJ_API UTestLevel : public UGameInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UTestLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelArena")
		void TestAll();
	UFUNCTION(BlueprintCallable, Category = "LevelArena")
		void TestArena(int count);
	bool TestLevelArena(int count);
	int Actors();
	int BlockingVolumes();
	int StaticMeshActors();
	int DirectionalLights();
	int ExponentialHeightFogs();
	int SkyLights();
	int PlayerStarts();

	//UFUNCTION(BlueprintCallable, Category = "LevelArena")
	TArray<int> GetObjectList() { return objectList; }
	//UFUNCTION(BlueprintCallable, Category = "LevelArena")
	int GetNbTest() { return nbTest; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LevelArena")
		TArray<int> objectList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LevelArena")
		int nbTest = 0;

};
