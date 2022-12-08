#pragma once

#include <stack>

#include <Engine/Classes/GameFramework/Actor.h>

#include <Core/Public/Math/MathFwd.h>

namespace AtomDestiny
{
    ///
    /// The Pool class represents the pool for a particular blueprint class.
    ///
    class ATOMDESTINY_API Pool
    {
    public:
        explicit Pool(TStrongObjectPtr<AActor> object);

        // Spawns an object from our pool
        TStrongObjectPtr<AActor> Spawn(FVector position, FRotator rotation);
        
        // Returns an object to the inactive pool.
        void Despawn(TStrongObjectPtr<AActor> object);
    
    private:
        
        // We would append an id to the name of anything we instantiate.
        int32_t m_nextId = 1;
        
        std::stack<TStrongObjectPtr<AActor>> m_inactive;
        TStrongObjectPtr<AActor> m_gameObjectToSpawn;
    };
    
} // namespace AtomDestiny
