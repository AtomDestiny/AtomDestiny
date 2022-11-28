#pragma once

#include "ActorPoolMember.h"

namespace AtomDestiny
{
    ///
    /// Core of Atom destiny object spawning instrument.
    /// Realized as singleton.
    ///
    class ATOMDESTINY_API ActorPool
    {
        const uint32_t DefaultPoolSize = 3;
    };
    
    // Redefinition to support classic pattern
    using ObjectPool = ActorPool;
}
