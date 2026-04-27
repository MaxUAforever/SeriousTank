// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineAdapterSubsystem : ModuleRules
{
	public OnlineAdapterSubsystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
                "Engine",
                "OnlineSubsystem",
                "OnlineSubsystemSteam",
                "OnlineSubsystemNull",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
	
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
			}
			);
	}
}
