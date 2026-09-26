#include "Despawner.h"
#include "ActorPool.h"

#include "TimerManager.h"
#include "Engine/World.h"

using namespace AtomDestiny;

void UDespawner::Despawn(double time)
{
    UWorld* world = GetWorld();
    if (world == nullptr)
    {
        return;
    }

    FTimerManager& timerManager = world->GetTimerManager();
    const TWeakObjectPtr<UDespawner> weakThis(this);

    const auto despawnHandler = [weakThis]
    {
        if (!weakThis.IsValid())
        {
            return;
        }

        AActor* owner = weakThis->GetOwner();
        if (!IsValid(owner))
        {
            return;
        }

        ObjectPool::Instance().Despawn(MakeWeakObjectPtr(owner));
    };

    constexpr bool noLoop = false;

    timerManager.ClearTimer(m_timerHandle);
    timerManager.SetTimer(m_timerHandle, despawnHandler, time, noLoop);
}

void UDespawner::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    ClearDespawnTimer();
    Super::EndPlay(endPlayReason);
}

void UDespawner::ClearDespawnTimer()
{
    if (UWorld* world = GetWorld())
    {
        world->GetTimerManager().ClearTimer(m_timerHandle);
    }
}
