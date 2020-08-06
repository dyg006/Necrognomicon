// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Necrognomicon : ModuleRules
{
	public Necrognomicon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", 
            "Slate", "SlateCore", "ProceduralMeshComponent", "ApexDestruction", "AIModule", "GameplayTasks",
            "NavigationSystem", "CableComponent" });
		PrivateDependencyModuleNames.AddRange(new string[] { "FMODStudio" });
	}
}
