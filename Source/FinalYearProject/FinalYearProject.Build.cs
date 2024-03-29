// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FinalYearProject : ModuleRules
{
	public FinalYearProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        OptimizeCode = CodeOptimization.Never;

    }
}
