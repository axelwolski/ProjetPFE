// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicAnalysis.h"


float UMusicAnalysis::GetBpm(FString SourceName)
{
	uint_t HopSize = 1024;
  uint_t WinSize = 1024;
  uint_t Framerate = 44100;
  uint_t Read = 0;

  FString SourcePath = FPaths::ProjectDir() + "Content/Music/" + SourceName;
	char_t* Source_path = TCHAR_TO_ANSI(*SourcePath);

	aubio_source_t* Source = new_aubio_source(Source_path, Framerate, HopSize);
  if (!Source)
    return 0.f;

	fvec_t* In = new_fvec(HopSize); // input audio buffer
  fvec_t* Out = new_fvec(1); // output position

	aubio_tempo_t * Tempo = new_aubio_tempo("default", WinSize, HopSize, Framerate);
  uint_t BpmMean = 0;
	do
  {
    aubio_source_do(Source, In, &Read);
    aubio_tempo_do(Tempo, In, Out);

    if(Out->data[0] != 0)
    {
      return aubio_tempo_get_bpm(Tempo);
    }
  }while(Read == HopSize);

	return 0.f;
}
