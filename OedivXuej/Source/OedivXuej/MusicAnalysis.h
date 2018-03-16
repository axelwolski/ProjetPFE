// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OedivXuej.h"
#define AUBIO_UNSTABLE 1
#include <aubio/aubio.h>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MusicAnalysis.generated.h"

/**
 *
 */
UCLASS()
class OEDIVXUEJ_API UMusicAnalysis : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MusicAnalysis")
		static float GetBpm(FString SourceName);
};
