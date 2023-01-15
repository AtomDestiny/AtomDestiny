#include "PlasmaShotProjectile.h"

#include <AtomDestiny/AtomDestinyGameStateBase.h>

#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

UPlasmaShotProjectile::UPlasmaShotProjectile(const FObjectInitializer& objectInitializer):
    UProjectileBase(objectInitializer)
{
}

void UPlasmaShotProjectile::Launch()
{
    GenerateImpact();
}

FAsyncCoroutine UPlasmaShotProjectile::GenerateImpact()
{
    co_await Coroutines::Latent::Seconds(m_damageDelay);

    if (m_parameters.target != nullptr)
    {
        AtomDestiny::GetGameState(GetOwner())->AddDamage(GET_INTERFACE(Projectile), EProjectileDamageOptions::ImpactPoint);
        AtomDestiny::ObjectPool::Instance().Spawn(m_impactBlueprint, m_points.impactPosition, FQuat::Identity);
    }

    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(GetOwner()));
}
