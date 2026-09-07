#pragma once

#include "UnitPoolAcquireMode.generated.h"

///
/// How a unit was acquired from the object pool.
///
UENUM(BlueprintType)
enum class EUnitPoolAcquireMode : uint8
{
    SetupPlacement,
    Battle
};
