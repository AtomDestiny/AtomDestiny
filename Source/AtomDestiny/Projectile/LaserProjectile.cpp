#include "Projectile/LaserProjectile.h"

#include <AtomDestiny/AtomDestinyGameStateBase.h>

#include <Niagara/Public/NiagaraComponent.h>

#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

ULaserProjectile::ULaserProjectile(const FObjectInitializer& objectInitializer):
    UProjectileBase(objectInitializer)
{
    m_niagaraComponent = objectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("NiagaraComponent"));
}

void ULaserProjectile::Launch()
{
    m_niagaraComponent->SetVariableVec3("BeamStartW", GetPoints().startPosition);
    m_niagaraComponent->SetVariableVec3("BeamEndW", GetPoints().impactPosition);
    m_niagaraComponent->SetVariableFloat("BeamLifeTime", m_lifeTime);

    check(m_damageDelay < m_lifeTime);
    GenerateImpact();

    AtomDestiny::ObjectPool::Instance().Despawn(MakeWeakObjectPtr(GetOwner()), m_lifeTime);
}

FAsyncCoroutine ULaserProjectile::GenerateImpact()
{
    co_await Coroutines::Latent::Seconds(m_damageDelay);

    if (m_parameters.target != nullptr)
    {
        AtomDestiny::GetGameState(GetOwner())->AddDamage(GET_INTERFACE(Projectile), EProjectileDamageOptions::ImpactPoint);
        AtomDestiny::ObjectPool::Instance().Spawn(m_impactPrefab, m_points.impactPosition, FQuat::Identity);
    }
}
