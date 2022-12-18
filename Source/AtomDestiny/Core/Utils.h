#pragma once

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

namespace AtomDestiny::Utils
{
    // Preloads blueprint (Prefab) to object pool from component
    inline void PreloadBlueprint(const TWeakObjectPtr<AActor>& blueprint, const uint32_t count)
    {
        if (blueprint.IsValid())
        {
            ObjectPool::Instance().Preload(blueprint, count);
        }
    }
    
} // namespace AtomDestiny::Utils
