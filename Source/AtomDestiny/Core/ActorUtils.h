#pragma once

#include <cassert>

#include <GameFramework/Actor.h>
#include <AtomDestiny/Core/Details/ActorUtilsHelper.h>

namespace AtomDestiny::Utils
{
    ///
    /// Sets actor enabled or disabled.
    /// Common interface to Unity.
    ///
    inline void SetActorActive(AActor* actor, const bool status)
    {
        assert(actor != nullptr);

        actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        if (!status)
        {
            actor->SetActorHiddenInGame(true);
            actor->SetActorEnableCollision(false);
            actor->SetActorTickEnabled(false);

            for (const auto component : actor->GetComponents())
            {
                Details::SetSimulatePhysics(component, false);
                component->SetActive(false);
            }
        }
        else
        {
            actor->SetActorEnableCollision(true);
            actor->SetActorTickEnabled(true);

            for (const auto component : actor->GetComponents())
            {
                component->SetActive(true);
            }

            // Visibility callbacks (AParticle, ACoroutineActor) must run after components are active.
            actor->SetActorHiddenInGame(false);
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
