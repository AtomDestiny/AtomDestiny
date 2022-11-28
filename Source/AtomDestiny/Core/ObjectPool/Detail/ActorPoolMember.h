#pragma once

#include <Core/Public/Templates/SharedPointer.h>

#include <Engine/Classes/Components/ActorComponent.h>

#include "ActorPoolMember.generated.h"

namespace AtomDestiny
{
    class Pool;
    
    ///
    /// Implements a member of AActor to control actor's pool>
    /// Added to freshly instantiated objects, so we can link back
    /// to the correct pool to UnSpawn.
    ///
    UCLASS()
    class UActorPoolMember final : public UActorComponent
    {
        GENERATED_BODY()

    public:
        TWeakPtr<Pool> pool;
    };
    
} // namespace AtomDestiny
