// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Shoot_Them_Up_2 : ModuleRules
{
	public Shoot_Them_Up_2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "Niagara", "PhysicsCore", "GameplayTasks", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[]
        { "Shoot_Them_Up_2/Public/Player",
            "Shoot_Them_Up_2/Public/Components",
            "Shoot_Them_Up_2/Public/Dev",
            "Shoot_Them_Up_2/Public/Weapon",
            "Shoot_Them_Up_2/Public/UI",
            "Shoot_Them_Up_2/Public/Animations",
            "Shoot_Them_Up_2/Public/Pickups",
            "Shoot_Them_Up_2/Public/Weapon/Components",
            "Shoot_Them_Up_2/Public/Ai",
            "Shoot_Them_Up_2/Public/Ai/Tasks",
            "Shoot_Them_Up_2/Public/Ai/Services"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
