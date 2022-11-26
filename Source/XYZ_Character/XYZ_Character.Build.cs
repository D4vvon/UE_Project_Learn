// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XYZ_Character : ModuleRules
{
	public XYZ_Character(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara", "UMG", "GameplayTasks", "NavigationSystem", "Slate", "SlateCore" });

		PrivateIncludePaths.AddRange(new string[] { Name });
	}
}
