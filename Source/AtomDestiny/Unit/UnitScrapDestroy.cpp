#include "UnitScrapDestroy.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/Logger.h>

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

    const AActor* owner = GetOwner();
    AtomDestiny::ObjectPool::Instance().Despawn(GetOwner());

    if (!IsValid(m_scrapBlueprint))
    {
        LOG_ERROR(TEXT("Scrap blueprint is empty"));
        return;
    }
    
    const TWeakObjectPtr<AActor> scrap = AtomDestiny::ObjectPool::Instance().Spawn(
        m_scrapBlueprint.GetDefaultObject(), owner->GetActorLocation(), owner->GetActorRotation().Quaternion());
    
    const TArray<UStaticMeshComponent*> components = AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(scrap.Get());
    
    for (UStaticMeshComponent* component : components)
    {
        const double power = FMath::FRandRange(m_minExplosionPower, m_maxExplosionPower);
        component->AddRadialImpulse(owner->GetActorLocation(), m_explosionRadius, static_cast<float>(power),
                                    ERadialImpulseFalloff::RIF_Constant, true);
    }

    RenderDebugSphere();
    AtomDestiny::ObjectPool::Instance().Despawn(scrap, m_partsDestroyTime);
}

void UUnitScrapDestroy::RenderDebugSphere()
{
    if (!m_renderExplosionSphere)
    {
        return;
    }

#ifdef ENABLE_DRAW_DEBUG
    DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), m_explosionRadius, 50, FColor::Cyan, false, m_partsDestroyTime + 1.0f);
#endif // ENABLE_DRAW_DEBUG
}
