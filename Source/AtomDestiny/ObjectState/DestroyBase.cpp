#include "DestroyBase.h"

#include <AtomDestiny/Core/Utils.h>

void UDestroyBase::BeginPlay()
{
    AtomDestiny::Utils::PreloadBlueprint(m_explosionBlueprint, BlueprintExplosionPreloadCount);
}
