#include "UnitScrapDestroy.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

UUnitScrapDestroy::UUnitScrapDestroy(const FObjectInitializer& objectInitializer):
    UDestroyBase(objectInitializer)
{
}

void UUnitScrapDestroy::BeginPlay()
{
    Super::BeginPlay();

    AtomDestiny::ObjectPool::Instance().Preload(m_scrapBlueprint, ScrapPreloadCount);
}

void UUnitScrapDestroy::Destroy()
{
    Super::Destroy();
    SpawnExplosion(GetOwner()->GetTransform().GetLocation(), FQuat::Identity);

    if (IsValid(m_scrapBlueprint))
    {
        const AActor* owner = GetOwner();
        const TWeakObjectPtr<AActor> scrap = AtomDestiny::ObjectPool::Instance().Spawn(m_scrapBlueprint.GetDefaultObject(), owner->GetActorLocation(), owner->GetActorRotation().Quaternion());

        if (scrap.IsValid())
        {
            const TArray<UStaticMeshComponent*> components = AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(scrap.Get());

            for (UStaticMeshComponent* component : components)
            {
                const double power = FMath::FRandRange(m_minExplosionPower, m_maxExplosionPower);
                component->AddRadialImpulse(component->GetComponentLocation(), m_explosionRadius, static_cast<float>(power), ERadialImpulseFalloff::RIF_Constant, true);
            }
        }
    }
}
