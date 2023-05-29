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
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

        CppStandard = CppStandardVersion.Latest;

        if (Platform == UnrealTargetPlatform.Mac || Platform == UnrealTargetPlatform.Linux)
        {
            AdditionalCompilerArguments += "-Wno-error=deprecated-anon-enum-enum-conversion -Wno-shadow -Wno-deprecated-declarations";
            AdditionalCompilerArguments += " -Wno-deprecated-anon-enum-enum-conversion";
            AdditionalCompilerArguments += " -fcoroutines-ts";
        }
        else if (Platform == UnrealTargetPlatform.Win64)
        {
            AdditionalCompilerArguments += "/wd4250";
            WindowsPlatform.CompilerVersion = "Latest";
        }

        ExtraModuleNames.Add("AtomDestiny");
    }
}
