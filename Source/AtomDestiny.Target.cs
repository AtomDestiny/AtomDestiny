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
        AdditionalCompilerArguments += "-Wno-error";
        Console.WriteLine("Compiler flags: " + AdditionalCompilerArguments);
        ExtraModuleNames.Add("AtomDestiny");
    }
}
