#include "DestroyBase.h"

#include <AtomDestiny/Core/Utils.h>

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
    AtomDestiny::Utils::PreloadBlueprint(m_explosionBlueprint, BlueprintExplosionPreloadCount);
}
