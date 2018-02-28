// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OedivXuej.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ErrorLog.generated.h"

/**
 *
 */
UCLASS()
class OEDIVXUEJ_API UErrorLog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Error")
    static bool WriteError(FString Type, FString ErrorText);
};
