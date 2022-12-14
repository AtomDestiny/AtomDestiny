// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AtomDestinyTarget : TargetRules
{
	public AtomDestinyTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		CppStandard = CppStandardVersion.Cpp20;
		ExtraModuleNames.Add("AtomDestiny");
	}
}
