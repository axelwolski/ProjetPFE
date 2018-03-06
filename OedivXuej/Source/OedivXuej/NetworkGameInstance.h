// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OedivXuej.h"
#include "Engine/GameInstance.h"
#include "NetworkGameInstance.generated.h"

/**
*
*/
UCLASS()
class OEDIVXUEJ_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UNetworkGameInstance(const FObjectInitializer& ObjectInitializer);

	// ***** blueprints ******
	UPROPERTY(BlueprintReadWrite, Category="Network")
		bool bMultiPlayerGame;
	UPROPERTY(BlueprintReadWrite, Category="Network")
		int MaxNumPlayers;
};
