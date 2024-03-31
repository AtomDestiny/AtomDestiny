#pragma once

#include "ProjectileBase.h"

#include "RocketBase.generated.h"

///
/// Represents basic rocket.
/// It contains all common parameters to rocket and missile.
///
UCLASS(Abstract)
class ATOMDESTINY_API ARocketBase : public AProjectileBase
{
    GENERATED_BODY()

public:
    explicit ARocketBase(const FObjectInitializer& objectInitializer);
};
