#include "Particle.h"

#include <Components/SceneComponent.h>
#include <Niagara/Public/NiagaraComponent.h>

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

AParticle::AParticle(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
{
    if (RootComponent == nullptr)
    {
        m_sceneComponent = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
        RootComponent = m_sceneComponent;
    }

    m_niagaraComponent = objectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("NiagaraComponent"));
    m_niagaraComponent->SetUsingAbsoluteLocation(true);

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AParticle::SetActorHiddenInGame(bool hidden)
{
    Super::SetActorHiddenInGame(hidden);

    if (!hidden)
    {
        UpdateNiagaraTransform();
        m_niagaraComponent->ActivateSystem();
        
        AtomDestiny::ObjectPool::Instance().Despawn(this, m_despawnDelay);
    }
}

void AParticle::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    UpdateNiagaraTransform();
}

void AParticle::UpdateNiagaraTransform()
{
    m_niagaraComponent->SetWorldLocation(GetActorLocation());
    m_niagaraComponent->SetWorldRotation(GetActorRotation());
}
