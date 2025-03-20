// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuestSubsystem: ModuleRules
{
	public QuestSubsystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "QuestSubsystem/Public"
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
                "DeveloperSettings",
                "Engine",
				"UMG",
                "Json"
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
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
