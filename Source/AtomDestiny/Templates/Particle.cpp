#include "Particle.h"

#include <Components/SceneComponent.h>
#include <Niagara/Public/NiagaraComponent.h>

AParticle::AParticle(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
{
    if (RootComponent == nullptr)
    {
        m_sceneComponent = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
        RootComponent = m_sceneComponent;
    }

    m_niagaraComponent = objectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("NiagaraComponent"));
}
