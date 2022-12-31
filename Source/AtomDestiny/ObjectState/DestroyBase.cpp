#include "DestroyBase.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

UDestroyBase::UDestroyBase(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
}

bool UDestroyBase::IsDestroyed() const
{
    return m_destroyed;
}

void UDestroyBase::Destroy()
{
    m_destroyed = true;
}

void UDestroyBase::BeginPlay()
{
    Super::BeginPlay();
    
    AtomDestiny::ObjectPool::Instance().Preload(m_explosionBlueprint, BlueprintExplosionPreloadCount);
}
