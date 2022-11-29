#pragma once

#include <Engine/Classes/Components/ActorComponent.h>

namespace AtomDestiny
{
    ///
    /// Actor despawner by timer.
    /// Actor would be despawned to Actor's Pool
    ///
    class ATOMDESTINY_API UDespawner final : public UActorComponent
    {
    public:
        void Despawn(double time);

    private:
        FTimerHandle m_timerHandle;
    };

    using Despawner = UDespawner;
    
} // namespace AtomDestiny
