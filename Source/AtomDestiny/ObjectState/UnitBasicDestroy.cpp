#include "UnitBasicDestroy.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

void UUnitBasicDestroy::Destroy()
{
    if (!m_explosionBlueprint.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit base destroy has invalid explosion blueprint"));
        return;
    }
    
    AtomDestiny::ObjectPool::Instance().Spawn(m_explosionBlueprint, GetOwner()->GetTransform().GetLocation(), FQuat::Identity);
    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(GetOwner()), m_deathDelay);
}
