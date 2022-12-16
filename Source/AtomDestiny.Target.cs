// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
using System.Collections.Generic;

public class AtomDestinyTarget : TargetRules
{
    public AtomDestinyTarget( TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;

        CppStandard = CppStandardVersion.Latest;
        bEnableCppCoroutinesForEvaluation = true;

        if (Platform == UnrealTargetPlatform.Mac)
        {
            AdditionalCompilerArguments += "-Wno-error=deprecated-anon-enum-enum-conversion -Wno-shadow -Wno-deprecated-declarations";
            AdditionalCompilerArguments += " -Wno-deprecated-anon-enum-enum-conversion";
            AdditionalCompilerArguments += " -stdlib=libc++ -fcoroutines-ts";
        }

        ExtraModuleNames.Add("AtomDestiny");
    }
}
