#include "PlasmaShotProjectile.h"

#include <AtomDestiny/AtomDestinyGameStateBase.h>

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

void UPlasmaShotProjectile::Launch()
{
    GenerateImpact();
}

FAsyncCoroutine UPlasmaShotProjectile::GenerateImpact()
{
    co_await UE5Coro::Latent::Seconds(m_damageDelay);

    if (m_parameters.target != nullptr)
    {
        AtomDestiny::GetGameState(GetOwner())->AddDamage(GET_INTERFACE(Projectile), EProjectileDamageOptions::ImpactPoint);
        AtomDestiny::ObjectPool::Instance().Spawn(m_impactBlueprint, m_points.impactPosition, FQuat::Identity);
    }

    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(GetOwner()));
}
