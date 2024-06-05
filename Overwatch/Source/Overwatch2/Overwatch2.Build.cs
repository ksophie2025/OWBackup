// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Overwatch2 : ModuleRules
{
	public Overwatch2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
