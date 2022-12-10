﻿#pragma once

#include <AtomDestiny/ObjectState/DestroyBase.h>

#include "UnitBasicDestroy.generated.h"

/// 
/// Standard unit destroy.
/// It just spawns destroy explosion blueprint.
///
UCLASS(Blueprintable)
class ATOMDESTINY_API UUnitBasicDestroy final : public UDestroyBase
{
    GENERATED_BODY()

public:

    // Destroys current actor
    virtual void Destroy() override;
    
protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Death delay"))
    double m_deathDelay = 0.2;
};