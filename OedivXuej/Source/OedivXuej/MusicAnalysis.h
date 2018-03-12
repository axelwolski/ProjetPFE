// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OedivXuej.h"
#include "aubio/aubio.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MusicAnalysis.generated.h"

/**
 *
 */
UCLASS()
class OEDIVXUEJ_API UMusicAnalysis : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="MusicAnalysis")
    static int GetBpm(FString SourcePath);
};
