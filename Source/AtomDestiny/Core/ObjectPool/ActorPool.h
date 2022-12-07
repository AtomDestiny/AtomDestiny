#pragma once

#include <unordered_map>

#include "Detail/Pool.h"

#include "AtomDestiny/Core/Hash.h"

namespace AtomDestiny
{
    ///
    /// Core of Atom destiny object spawning instrument.
    /// Realized as singleton.
    ///
    class ATOMDESTINY_API ActorPool
    {
        // private for singleton
        ActorPool() = default;
        ~ActorPool() = default;

    public:
        static ActorPool& Instance();
        
        // Initializes our dictionary
        void Initialize(GameObject object);
        
        // Spawns a copy of the specified actor (instantiating one if required)
        GameObject Spawn(GameObject object, FVector position, FRotator rotation);
        
        // Spawns a copy of the specified prefab, with zero pos and identity rotation
        GameObject Spawn(GameObject object);
        
        // Despawns the specified AActor back into its pool.
        void Despawn(GameObject object) const;
        
        // Despawns the specified AActor back into its pool after time
        void Despawn(GameObject object, double time) const;
        
        // Cleans all pooled members
        void DestroyAll();
        
        // Destroys current object
        void Destroy(GameObject object);
        
        // Returns true if Blueprint Actor is already in pool
        bool Contains(GameObject object) const;
    
    private:
        std::unordered_map<GameObject, TSharedPtr<Pool>> m_pools;
    };

    // Redefinition to support classic pattern
    using ObjectPool = ActorPool;

} // namespace AtomDestiny
