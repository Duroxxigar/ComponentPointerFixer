// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ComponentPointerFixer : ModuleRules
{
	public ComponentPointerFixer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "CoreUObject", "Engine", "InputCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd", 
				"EditorStyle", 
				"PropertyEditor", 
				"Slate", 
				"SlateCore", 
				"SubobjectEditor", 
				"SubobjectDataInterface", 
				"ToolMenus",
				"Kismet" 
			}
		);
	}
}