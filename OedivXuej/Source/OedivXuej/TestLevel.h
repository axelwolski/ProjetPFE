// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class OEDIVXUEJ_API TestLevel
{
public:
	TestLevel();
	~TestLevel();
	int Actors();
	int BlockingVolumes();
	int StaticMeshActors();
	int DirectionalLight();
	int ExponentialHeightFog();
	int SkyLight();
	int PlayerStart();
};
