#include "UnitBasicDestroy.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

void UUnitBasicDestroy::Destroy()
{
    if (!m_explosionBlueprint.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit base destroy has invalid explosion blueprint"));
        return;
    }
    
    FRotator rotator { FQuat::Identity };
    TStrongObjectPtr blueprintPtr { m_explosionBlueprint.Get() };
    
    AtomDestiny::ObjectPool::Instance().Spawn(std::move(blueprintPtr), GetOwner()->GetTransform().GetLocation(), std::move(rotator));

    TStrongObjectPtr currentActor { GetOwner() };
    AtomDestiny::ObjectPool::Instance().Despawn(std::move(currentActor), m_deathDelay);
}
