#pragma once

#include <cassert>

#include <Engine/Classes/GameFramework/Actor.h>

namespace AtomDestiny::Utils
{
    ///
    /// Sets actor enabled or disabled.
    /// Common interface to Unity.
    ///
    inline void SetActorActive(AActor* actor, const bool status)
    {
        assert(actor != nullptr);
        
        actor->SetActorHiddenInGame(!status);
        actor->SetActorEnableCollision(status);
        actor->SetActorTickEnabled(status);

        for (const auto component : actor->GetComponents())
        {
            component->SetActive(status);
        }
    }

    inline void SetActorActive(const TStrongObjectPtr<AActor>& object, const bool status)
    {
        SetActorActive(object.Get(), status);
    }

    inline void SetActorActive(const TWeakObjectPtr<AActor>& object, const bool status)
    {
        SetActorActive(object.Get(), status);
    }
    
} // namespace AtomDestiny::Utils
