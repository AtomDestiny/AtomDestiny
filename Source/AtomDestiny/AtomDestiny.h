#pragma once

#include "CoreMinimal.h"
#include <Templates/UnrealTemplate.h>

UENUM(BlueprintType)
enum class EGameSide : uint8
{
    Rebels,
    Federation,
    Neutral,
    None,
};

//
// We should use vector instead of hash because units would mostly iterate through this list.
// Search, addition and removing is not a super frequency operation.
//
using FGameStateUnitList = TArray<TWeakObjectPtr<AActor>>;
using FSharedGameStateUnitList = TSharedPtr<FGameStateUnitList>;
using FEnemiesList = TArray<FSharedGameStateUnitList>;

namespace AtomDestiny
{
    struct GameSide
    {
        static FString ToString(EGameSide);
    };
    
} // namespace AtomDestiny
