// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class d3d_ue5 : ModuleRules
{
	public d3d_ue5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "ProceduralMeshComponent", "DeveloperSettings"});
	}
}
