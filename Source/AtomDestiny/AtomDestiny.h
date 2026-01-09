#pragma once

#include <CoreMinimal.h>
#include <Templates/UnrealTemplate.h>

#include <GameFramework/Actor.h>

UENUM(BlueprintType)
enum class EGameSide : uint8
{
    Rebels = 0,
    Federation,
    Neutral,
    None,
};

ENUM_RANGE_BY_COUNT(EGameSide, static_cast<uint8>(EGameSide::None))

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
        // return all enum side names
        static const TArray<FName>& GetNames();
        static FString ToString(EGameSide);
        static FName ToName(EGameSide);
    };

} // namespace AtomDestiny
