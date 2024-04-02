#include "RocketBase.h"

#include <Components/BoxComponent.h>

#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

#include <AtomDestiny/AtomDestinyGameStateBase.h>

ARocketBase::ARocketBase(const FObjectInitializer& objectInitializer):
    AProjectileBase(objectInitializer)
{
    if (RootComponent == nullptr)
    {
        m_boxCollider = objectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollider"));
        m_boxCollider->SetGenerateOverlapEvents(true);
        m_boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        
        RootComponent = m_boxCollider;
    }
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

FAsyncCoroutine ARocketBase::LockOn()
{
    if (m_activationTime <= m_lockedTime)
    {
        co_await Coroutines::Latent::Seconds(m_activationTime);
        m_launched = true;

        co_await Coroutines::Latent::Seconds(m_lockedTime - m_activationTime);
        m_locked = true;
    }
    else
    {
        co_await Coroutines::Latent::Seconds(m_lockedTime);
        m_locked = true;

        co_await Coroutines::Latent::Seconds(m_activationTime - m_lockedTime);
        m_launched = true;
    }
}

void ARocketBase::NotifyActorBeginOverlap(AActor*)
{
    if (!m_launched)
        return;

    AtomDestiny::GetGameState(this)->AddDamage(this, EProjectileDamageOptions::ProjectilePoint);    // TODO: mb use Impact point instead
    AtomDestiny::ObjectPool::Instance().Spawn(m_impactPrefab, m_points.impactPosition, FQuat::Identity);

    AtomDestiny::ObjectPool::Instance().Despawn(this);
}
