#include "PlasmaShotProjectile.h"

#include <AtomDestiny/AtomDestinyGameStateBase.h>

#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

APlasmaShotProjectile::APlasmaShotProjectile(const FObjectInitializer& objectInitializer):
    AProjectileBase(objectInitializer)
{
}

void APlasmaShotProjectile::Launch()
{
    GenerateImpact();
}

FAsyncCoroutine APlasmaShotProjectile::GenerateImpact()
{
    co_await Coroutines::Latent::Seconds(m_damageDelay);

    if (m_parameters.target.IsValid())
    {
        AtomDestiny::GetGameState(GetOwner())->AddDamage(this, EProjectileDamageOptions::ImpactPoint);
        AtomDestiny::ObjectPool::Instance().Spawn(m_impactPrefab, m_points.impactPosition, FQuat::Identity);
    }

    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(this));
}
