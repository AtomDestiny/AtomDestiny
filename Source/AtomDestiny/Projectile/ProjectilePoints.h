#pragma once

#include <Runtime/Core/Public/Math/MathFwd.h>

#include "ProjectilePoints.generated.h"

USTRUCT()
struct FProjectilePoints
{
    GENERATED_BODY()

    FVector startPosition;
    FVector endPosition;
    FVector impactPosition;
    FVector normal;
};
