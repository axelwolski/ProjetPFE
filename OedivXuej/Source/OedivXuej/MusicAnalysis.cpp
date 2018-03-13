// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicAnalysis.h"


int UMusicAnalysis::GetBpm(FString SourcePath)
{
	char_t* source_path = "bonjour";
	aubio_source_t* Source = new_aubio_source(source_path, 44100, 1024);
	return 0;
}

