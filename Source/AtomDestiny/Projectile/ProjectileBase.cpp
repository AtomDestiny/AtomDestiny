#include "ProjectileBase.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/ActorUtils.h>

#include <AtomDestiny/Logic/Logic.h>

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

void AProjectileBase::Launch()
{
    if (m_parameters.owner.IsValid())
    {
        if (const auto logic = AtomDestiny::Utils::GetInterface<ILogic>(m_parameters.owner); logic != nullptr)
        {
            FName sideTag = AtomDestiny::GameSide::ToName(logic->GetSide());
            AtomDestiny::Utils::SetActorSideTag(m_parameters.owner, std::move(sideTag));
        }
    }
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
    AtomDestiny::ObjectPool::Instance().Preload(m_impactPrefab);
}
