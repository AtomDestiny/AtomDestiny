#include "InvisibleProjectile.h"

#include <AtomDestiny/AtomDestinyGameStateBase.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

AInvisibleProjectile::AInvisibleProjectile(const FObjectInitializer& objectInitializer):
    AProjectileBase(objectInitializer)
{
}

void AInvisibleProjectile::Launch()
{
    RunCoroutine(GenerateImpact());
}

FAsyncCoroutine AInvisibleProjectile::GenerateImpact()
{
    co_await Coroutines::Latent::Seconds(m_damageDelay);

    if (m_parameters.target.IsValid())
    {
        AtomDestiny::GetGameState(this)->AddDamage(this, EProjectileDamageOptions::ImpactPoint);
        AtomDestiny::ObjectPool::Instance().Spawn(m_impactPrefab, m_points.impactPosition, FQuat::Identity);
    }

    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(this));
}
