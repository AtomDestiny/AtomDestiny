#pragma once

#include <cassert>

#include <GameFramework/Actor.h>

#include <AtomDestiny/AtomDestiny.h>
#include <AtomDestiny/Core/Details/ActorUtilsHelper.h>

namespace AtomDestiny::Utils
{
    ///
    ///  Sets actor active for editor purposes only
    ///
    inline void SetActorEditorActive(AActor* actor, const bool isActive)
    {
        assert(actor != nullptr);

        actor->SetActorEnableCollision(isActive);
        actor->SetActorTickEnabled(isActive);

        for (const auto component : actor->GetComponents())
        {
            if (!isActive)
            {
                Details::SetSimulatePhysics(component, false);
            }

            component->SetActive(isActive);
        }
    }

    ///
    /// Sets actor enabled or disabled.
    /// Common interface to Unity.
    ///
    inline void SetActorActive(AActor* actor, const bool isActive)
    {
        assert(actor != nullptr);

        actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        actor->SetActorHiddenInGame(!isActive);

        SetActorEditorActive(actor, isActive);
    }

    inline void SetActorActive(const TStrongObjectPtr<AActor>& object, const bool isActive)
    {
        SetActorActive(object.Get(), isActive);
    }

    inline void SetActorActive(const TWeakObjectPtr<AActor>& object, const bool isActive)
    {
        SetActorActive(object.Get(), isActive);
    }

    // we consider that currently actor tags must contain only one side name
    inline void SetActorSideTag(const TWeakObjectPtr<AActor>& object, FName&& sideTag)
    {
        TArray<FName>& tags = object->Tags;

        if (const auto index = tags.Find(sideTag); index != INDEX_NONE)
            return;

        for (const FName& name : GameSide::GetNames())
        {
            if (const auto index = tags.Find(name); index != INDEX_NONE)
            {
                tags.RemoveAt(index);
                break;
            }
        }

        object->Tags.Emplace(std::move(sideTag));
    }

    inline void SetActorTag(const TWeakObjectPtr<AActor>& object, FName&& tag)
    {
        if (object->Tags.Find(tag) != INDEX_NONE)
        {
            return;
        }

        object->Tags.Emplace(std::move(tag));
    }

} // namespace AtomDestiny::Utils
