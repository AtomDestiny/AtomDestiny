#pragma once

#include <stack>

#include "ActorPoolMember.h"

#include "AtomDestiny/Core/AtomDestinyCore.h"

#include <Core/Public/Math/MathFwd.h>

namespace AtomDestiny
{
    ///
    /// The Pool class represents the pool for a particular blueprint class.
    ///
    class ATOMDESTINY_API Pool
    {
    public:
        explicit Pool(GameObject object);

        // Spawns an object from our pool
        GameObject Spawn(FVector position, FRotator rotation);
        
        // Returns an object to the inactive pool.
        void Despawn(GameObject object);
    
    private:
        // We would append an id to the name of anything we instantiate.
        uint64_t m_nextId = 1;
        
        std::stack<GameObject> m_inactive;
        GameObject m_gameObjectToSpawn;
    };
    
} // namespace AtomDestiny
