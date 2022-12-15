using UnrealBuildTool;

public class AtomDestiny : ModuleRules
{
    public AtomDestiny(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "UMG",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks", "AIModule", "NavigationSystem"
        });

        PublicIncludePaths.AddRange(new string[] {
            "AtomDestiny"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        
         PrivateDependencyModuleNames.AddRange(new string[] {  /*"AIModule", "NavigationSystem"*/ });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        
        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
