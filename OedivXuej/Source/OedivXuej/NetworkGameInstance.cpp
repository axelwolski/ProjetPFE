// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"



UNetworkGameInstance::UNetworkGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bMultiPlayerGame(false), MaxNumPlayers(1)
{
}
