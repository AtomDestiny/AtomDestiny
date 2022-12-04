﻿#pragma once

#include "AtomDestiny/Core/AtomDestinyCore.h"

namespace AtomDestiny::Utils
{
    ///
    /// Sets actor enabled or disabled.
    /// Common interface to Unity.
    /// 
    inline void SetActorActive(AActor* actor, const bool status)
    {
        actor->SetActorHiddenInGame(!status);
        actor->SetActorEnableCollision(status);
        actor->SetActorTickEnabled(status);
    }

    inline void SetActorActive(const GameObject& object, const bool status)
    {
        SetActorActive(object.Get(), status);
    }
    
} // namespace AtomDestiny::Utils