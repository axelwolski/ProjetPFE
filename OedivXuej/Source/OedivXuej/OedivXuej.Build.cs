// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OedivXuej : ModuleRules
{
	public OedivXuej(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemUtils" });
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
	}
}
