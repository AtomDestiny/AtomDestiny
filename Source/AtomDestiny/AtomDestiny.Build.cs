using UnrealBuildTool;

public class AtomDestiny : ModuleRules
{
    public AtomDestiny(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Latest;
        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "UMG",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "UE5Coro",
            "Niagara",
            "NiagaraCore"
        });

        PublicIncludePaths.AddRange(new string[] {
            "AtomDestiny"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });
        PrivateDependencyModuleNames.AddRange(new string[] {  /*"AIModule", "NavigationSystem"*/ });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
