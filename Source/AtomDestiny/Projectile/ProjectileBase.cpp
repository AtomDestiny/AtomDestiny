#include "ProjectileBase.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

AProjectileBase::AProjectileBase(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
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

void AProjectileBase::PreInitializeComponents()
{
    Super::PreInitializeComponents();
    AtomDestiny::ObjectPool::Instance().Preload(m_impactPrefab);
}
