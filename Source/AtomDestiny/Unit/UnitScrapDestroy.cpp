#include "UnitScrapDestroy.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

UUnitScrapDestroy::UUnitScrapDestroy(const FObjectInitializer& objectInitializer):
    UDestroyBase(objectInitializer)
{
}

void UUnitScrapDestroy::BeginPlay()
{
    Super::BeginPlay();

    AtomDestiny::ObjectPool::Instance().Preload(m_scrapBlueprint, ScrapPreloadCount);
}

void UUnitScrapDestroy::Destroy()
{
    Super::Destroy();
    SpawnExplosion(GetOwner()->GetTransform().GetLocation(), FQuat::Identity);

    if (m_scrapBlueprint)
    {
        const AActor* owner = GetOwner();
        // AtomDestiny::ObjectPool::Instance().Spawn(m_scrapBlueprint.GetDefaultObject(),
        //     owner->GetActorLocation(), owner->GetActorRotation());
        // for (const UActorComponent* component : m_)
    }
}
