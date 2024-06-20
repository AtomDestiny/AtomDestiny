#include "ProjectileBase.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

AProjectileBase::AProjectileBase(const FObjectInitializer& objectInitializer):
    ACoroutineActor(objectInitializer)
{
}

void AProjectileBase::SetPoints(const FProjectilePoints& points)
{
    m_points = points;
}

void AProjectileBase::SetParameters(const FWeaponParameters& parameters)
{
    m_parameters = parameters;
}

const FWeaponParameters& AProjectileBase::GetParameters() const
{
    return m_parameters;
}

const FProjectilePoints& AProjectileBase::GetPoints() const
{
    return m_points;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
    AtomDestiny::ObjectPool::Instance().Preload(m_impactPrefab);
}
