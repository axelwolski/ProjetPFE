// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicAnalysis.h"

int UMusicAnalysis::GetBpm(FString SourcePath)
{
  const char_t* SourceChar = TCHAR_TO_ANSI(*SourcePath);
  aubio_source_t * Source = new_aubio_source(SourceChar, 44100, 1024);
  return 0;
}
