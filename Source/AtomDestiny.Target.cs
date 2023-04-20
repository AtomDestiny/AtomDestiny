// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
using System.Collections.Generic;
using UnrealBuildBase;

public class AtomDestinyTarget : TargetRules
{
    public AtomDestinyTarget( TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;

        CppStandard = CppStandardVersion.Latest;

        if (Platform == UnrealTargetPlatform.Mac || Platform == UnrealTargetPlatform.Linux)
        {
            AdditionalCompilerArguments += "-Wno-error=deprecated-anon-enum-enum-conversion -Wno-shadow -Wno-deprecated-declarations";
            AdditionalCompilerArguments += " -Wno-deprecated-anon-enum-enum-conversion";
            AdditionalCompilerArguments += " -fcoroutines-ts";
        }

        ExtraModuleNames.Add("AtomDestiny");
    }
}
