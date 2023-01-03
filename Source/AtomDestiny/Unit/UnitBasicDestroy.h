﻿#pragma once

#include <AtomDestiny/Behaviour/DestroyBase.h>

#include "UnitBasicDestroy.generated.h"

/// 
/// Standard unit destroy.
/// It just spawns destroy explosion blueprint.
///
UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class ATOMDESTINY_API UUnitBasicDestroy final : public UDestroyBase
{
    GENERATED_BODY()

public:
    explicit UUnitBasicDestroy(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Destroys current actor
    virtual void Destroy() override;
};
