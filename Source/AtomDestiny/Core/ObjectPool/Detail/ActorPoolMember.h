#pragma once

#include <Core/Public/Templates/SharedPointer.h>

#include <Engine/Classes/Components/ActorComponent.h>

namespace AtomDestiny
{
    class Pool;
    
    ///
    /// Implements a member of AActor to control actor's pool>
    /// Added to freshly instantiated objects, so we can link back
    /// to the correct pool to UnSpawn.
    ///
    class ATOMDESTINY_API UActorPoolMember final : public UActorComponent
    {
    public:
        TSharedPtr<Pool> pool;
    };
    
    using ActorPoolMember = UActorPoolMember;
    
} // namespace AtomDestiny
