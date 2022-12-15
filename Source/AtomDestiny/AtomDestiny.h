#pragma once

#include "CoreMinimal.h"
#include <Core/Public/Templates/UnrealTemplate.h>

UENUM(BlueprintType)
enum class EGameSide : uint8
{
    None,
    Rebels,
    Federation,
    Neutral,
    NeutralBuildings,
};
