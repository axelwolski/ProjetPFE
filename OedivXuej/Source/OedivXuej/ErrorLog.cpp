// Fill out your copyright notice in the Description page of Project Settings.

#include "ErrorLog.h"

bool UErrorLog::WriteError(FString Type, FString ErrorText)
{
  FString FilePath = FPaths::ProjectDir() + "/" + "Error.log";
  FString SaveText = Type + LINE_TERMINATOR + ErrorText + LINE_TERMINATOR;
  return FFileHelper::SaveStringToFile(SaveText, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}
