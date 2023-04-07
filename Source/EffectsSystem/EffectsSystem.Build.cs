// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2022.

using UnrealBuildTool;

public class EffectsSystem : ModuleRules
{
	public EffectsSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.AddRange(
			new string[] 
			{
				"EffectsSystem/Public/"
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"NetCore",
				"DataSystem",
				"LogSystem"
			}
		);
		
		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"HealthDamageSystem",
				"DataSystem",
				"LogSystem"
			}
		);
	}
}
