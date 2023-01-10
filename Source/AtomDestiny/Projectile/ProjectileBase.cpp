#include "ProjectileBase.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Particle/Particle.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

UProjectileBase::UProjectileBase(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
}

void UProjectileBase::SetPoints(const FProjectilePoints& points)
{
    m_points = points;
}

void UProjectileBase::SetParameters(const FWeaponParameters& parameters)
{
    m_parameters = parameters;
}

const FWeaponParameters& UProjectileBase::GetParameters() const
{
    return m_parameters;
}

const FProjectilePoints& UProjectileBase::GetPoints() const
{
    return m_points;
}

void UProjectileBase::InitializeComponent()
{
    AtomDestiny::ObjectPool::Instance().Preload(m_impactBlueprint, ImpactPreloadCount);
}
