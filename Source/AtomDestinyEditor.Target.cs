// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AtomDestinyEditorTarget : TargetRules
{
    public AtomDestinyEditorTarget( TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        
        bOverrideBuildEnvironment = true;
        CppStandard = CppStandardVersion.Latest;

        if (Platform == UnrealTargetPlatform.Mac)
        {
            AdditionalCompilerArguments += "-Wno-error=deprecated-anon-enum-enum-conversion -Wno-shadow -Wno-deprecated-declarations";
            AdditionalCompilerArguments += " -Wno-deprecated-anon-enum-enum-conversion";
            AdditionalCompilerArguments += " -fcoroutines-ts";
        }

        ExtraModuleNames.Add("AtomDestiny");
    }
}
