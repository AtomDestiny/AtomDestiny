#include "UnitBasicDestroy.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/Logger.h>

UUnitBasicDestroy::UUnitBasicDestroy(const FObjectInitializer& objectInitializer):
    UDestroyBase(objectInitializer)
{
}

void UUnitBasicDestroy::Destroy()
{
    if (m_destroyed)
    {
        LOG_ERROR(TEXT("Unit has a destroy status already"));
        return;
    }
    
    if (!IsValid(m_explosionBlueprint))
    {
        LOG_WARNING(TEXT("Unit basic destroy has invalid explosion blueprint, so no explosion would be spawned"));
    }
    else
    {
        SpawnExplosion(GetOwner()->GetTransform().GetLocation(), FQuat::Identity);
    }

    Super::Destroy();
    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(GetOwner()));
}
