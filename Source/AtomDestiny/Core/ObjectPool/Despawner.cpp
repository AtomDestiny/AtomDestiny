#include "Despawner.h"
#include "ActorPool.h"

#include "TimerManager.h"
#include "Engine/World.h"

using namespace AtomDestiny;

UDespawner::UDespawner(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
    // Despawner is added at runtime, it should be active to receive Deactivate from the pool
    bAutoActivate = true;
}

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

void UDespawner::Deactivate()
{
    // Actor returned to pool, pending despawn must not hit the next reused instance
    ClearDespawnTimer();
    Super::Deactivate();
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
