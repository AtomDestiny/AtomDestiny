#pragma once

#include "AtomDestiny/AtomDestiny.h"
#include "AtomDestiny/Unit/Unit.h"

#include "TacticsLayoutElement.generated.h"

///
/// Struct for one unit that was placed on the grid before the battle (PreBattle layout)
///

USTRUCT(BlueprintType)
struct FTacticsLayoutElement
{
    GENERATED_BODY()

    UPROPERTY()
    EADUnitType unitType = EADUnitType::None;

    UPROPERTY()
    EGameSide side = EGameSide::None;

    UPROPERTY()
    FVector location = FVector::ZeroVector;

    UPROPERTY()
    FRotator rotation = FRotator::ZeroRotator;
};
