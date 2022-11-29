#include "Despawner.h"

#include "ActorPool.h"

using namespace AtomDestiny;

void UDespawner::Despawn(double time)
{
    decltype(auto) timerManager = GetWorld()->GetTimerManager();
    const auto despawnHandler = [this] {
        ObjectPool::Instance().Despawn(MakeShareable(this->GetOwner()));
    };
    
    constexpr bool noLoop = false;
    
    timerManager.ClearTimer(m_timerHandle);
    timerManager.SetTimer(m_timerHandle, despawnHandler, time, noLoop);
}
