#pragma once

#include <stack>
#include <memory>

#include <Engine/Classes/GameFramework/Actor.h>

#include <Core/Public/Math/MathFwd.h>

namespace AtomDestiny
{
    ///
    /// The Pool class represents the pool to increase spawn/despawn performance.
    ///
    class ATOMDESTINY_API Pool : public std::enable_shared_from_this<Pool>
    {
    public:
        explicit Pool(TWeakObjectPtr<AActor> object);

        // Spawns an object from our pool
        TWeakObjectPtr<AActor> Spawn(const FVector& position, const FQuat& rotation);
        
        // Returns an object to the inactive pool.
        void Despawn(TWeakObjectPtr<AActor> object);
    
    private:
        
        // Counter to append an id to the name of anything instantiated.
        int32_t m_nextId = 1;
        
        std::stack<TWeakObjectPtr<AActor>> m_inactive;
        TWeakObjectPtr<AActor> m_gameObjectToSpawn;
    };
    
} // namespace AtomDestiny
