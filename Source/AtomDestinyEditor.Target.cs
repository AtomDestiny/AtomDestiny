// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AtomDestinyEditorTarget : TargetRules
{
    public AtomDestinyEditorTarget( TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        
        bOverrideBuildEnvironment = true;
        CppStandard = CppStandardVersion.Latest;

        if (Platform == UnrealTargetPlatform.Mac)
        {
            AdditionalCompilerArguments += "-Wno-error=deprecated-anon-enum-enum-conversion -Wno-shadow -Wno-deprecated-declarations";
            AdditionalCompilerArguments += " -Wno-deprecated-anon-enum-enum-conversion";
            AdditionalCompilerArguments += " -fcoroutines-ts";
        }
        else if (Platform == UnrealTargetPlatform.Win64)
        {
            AdditionalCompilerArguments += "/wd4250";
        }

        ExtraModuleNames.Add("AtomDestiny");
    }
}
