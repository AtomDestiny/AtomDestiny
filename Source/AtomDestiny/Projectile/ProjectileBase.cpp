#include "ProjectileBase.h"

#include <AtomDestiny/Core/Utils.h>

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
    AtomDestiny::Utils::PreloadBlueprint(m_impactBlueprint, ImpactPreloadCount);
}
