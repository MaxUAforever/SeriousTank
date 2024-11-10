// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AIPatrollingSubsystem : ModuleRules
{
	public AIPatrollingSubsystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[]{});
		PrivateIncludePaths.AddRange(new string[]{});
			
		PublicDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine"});
		PrivateDependencyModuleNames.AddRange(new string[]{ "AIModule", "GameplayTasks" });
		
		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
