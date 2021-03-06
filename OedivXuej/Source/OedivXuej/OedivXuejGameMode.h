// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "OedivXuejGameMode.generated.h"

UCLASS(minimalapi)
class AOedivXuejGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOedivXuejGameMode();
	void PostLogin(APlayerController * NewPlayer);
	void HideDoor();

	AActor* ClosingDoor;
	AActor* DownDoor;
	TArray<AActor*> ActorList;

	FTimerHandle DoorDelayTimerHandle;
};
