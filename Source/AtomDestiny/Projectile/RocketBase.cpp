#include "RocketBase.h"

#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

ARocketBase::ARocketBase(const FObjectInitializer& objectInitializer):
    AProjectileBase(objectInitializer)
{
}

void ARocketBase::BeginPlay()
{
    Super::BeginPlay();

    m_rocketForward = GetActorForwardVector();
    m_startLifeTime = m_lifeTime;

    AtomDestiny::ObjectPool::Instance().Preload(m_tracePrefab);
}

void ARocketBase::OnDisabled()
{
    Super::OnDisabled();

    m_lifeTime = m_startLifeTime;
    m_locked = false;
    m_launched = false;

    SetActorRotation(m_rocketForward.Rotation());
    
    // reset rigid body
}

FAsyncCoroutine ARocketBase::LockOnDelay()
{
    co_await Coroutines::Latent::Seconds(m_lockedTime);
    m_locked = true;
}

FAsyncCoroutine ARocketBase::LaunchDelay()
{
    co_await Coroutines::Latent::Seconds(m_activationTime);
    m_launched = true;
}

