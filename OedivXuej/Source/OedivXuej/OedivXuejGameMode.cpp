// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OedivXuejGameMode.h"
#include "ErrorLog.h"
#include "OedivXuejCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOedivXuejGameMode::AOedivXuejGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AOedivXuejGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// destroy the arena blocking door when playing solo or all players are in the level
	TArray<AActor*> ActorList = GetWorld()->GetCurrentLevel()->Actors;
	UNetworkGameInstance* Gi = Cast<UNetworkGameInstance>(GetWorld()->GetGameInstance());
	if(Gi)
	{
		if(!Gi->bMultiPlayerGame || GetNumPlayers() == Gi->MaxNumPlayers)
		{
			if(GetWorld()->GetMapName().Mid(GetWorld()->StreamingLevelsPrefix.Len()) == "Arena_V1")
			{
				for (int32 i = 0; i != ActorList.Num(); i++)
				{
					if(ActorList[i]->GetName() == "ArenaV1_Lobby_Door_BP_180")
					{
						if(ActorList[i]->Destroy())
						{
							ActorList.RemoveAt(i);
						}
						else
						{
							UErrorLog::WriteError("PostLogin", "Can't destroy actor");
						}
					}
				}
			}
		}
	}
	else
	{
		UErrorLog::WriteError("PostLogin", "Can't find game instance");
	}
}
