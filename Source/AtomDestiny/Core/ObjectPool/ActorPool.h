#pragma once

#include <unordered_map>

#include "Details/Pool.h"

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

        const uint32_t DefaultPreloadCount = 3;
        
    public:
        static ActorPool& Instance();
        
        // Initializes our dictionary
        void Initialize(const TWeakObjectPtr<AActor>& object);
        
        // Spawns a copy of the specified actor (instantiating one if required)
        TWeakObjectPtr<AActor> Spawn(TWeakObjectPtr<AActor> object, const FVector& position, const FQuat& rotation);
        
        // Spawns a copy of the specified prefab, with zero pos and identity rotation
        TWeakObjectPtr<AActor> Spawn(TWeakObjectPtr<AActor> object);
        
        // Des-pawns the specified AActor back into its pool.
        void Despawn(TWeakObjectPtr<AActor> object) const;
        
        // Des-pawns the specified AActor back into its pool after time
        void Despawn(TWeakObjectPtr<AActor> object, double time) const;
        
        // Cleans all pooled members
        void DestroyAll();
        
        // Destroys current object
        void Destroy(TWeakObjectPtr<AActor> object);
        
        // Returns true if Blueprint Actor is already in pool
        bool Contains(TWeakObjectPtr<AActor> object) const;

        // Early objects preloading
        void Preload(const TSubclassOf<AActor>& object, uint32_t size = 1);
        
    private:
        void Preload(const TWeakObjectPtr<AActor>& object, uint32_t size = 1);
        
        bool m_preloadingActive = true;
        std::unordered_map<TWeakObjectPtr<AActor>, std::shared_ptr<Pool>> m_pools;
    };

    // Redefinition to support classic pattern
    using ObjectPool = ActorPool;

} // namespace AtomDestiny
