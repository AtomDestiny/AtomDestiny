#pragma once

#include "Detail/ActorPoolMember.h"

namespace AtomDestiny
{
    ///
    /// Core of Atom destiny object spawning instrument.
    /// Realized as singleton.
    ///
    class ATOMDESTINY_API ActorPool
    {
        const uint32_t DefaultPoolSize = 3;

        // private for singleton
        ActorPool() = default;
        ~ActorPool() = default;

    public:
        static ActorPool& Instance();

    private:
        bool m_preloadingActive = true;
    };

    // Redefinition to support classic pattern
    using ObjectPool = ActorPool;
    
} // namespace AtomDestiny
