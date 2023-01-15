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
    SpawnExplosion(GetOwner()->GetActorLocation(), FQuat::Identity);
    
    const FVector actorLocation = GetOwner()->GetActorLocation();
    const FQuat actorRotation = GetOwner()->GetActorQuat();
    
    AtomDestiny::ObjectPool::Instance().Despawn(GetOwner());

    if (!IsValid(m_scrapBlueprint))
    {
        LOG_ERROR(TEXT("Scrap blueprint is empty"));
        return;
    }
    
    const TWeakObjectPtr<AActor> scrap = AtomDestiny::ObjectPool::Instance().Spawn(m_scrapBlueprint.GetDefaultObject(), actorLocation, actorRotation);
    const TArray<UStaticMeshComponent*> components = AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(scrap.Get());
    
    for (const auto component : components)
    {
        const double power = FMath::FRandRange(m_minExplosionPower, m_maxExplosionPower);

        component->SetSimulatePhysics(true);
        component->AddRadialImpulse(scrap->GetActorLocation(), m_explosionRadius, static_cast<float>(power), ERadialImpulseFalloff::RIF_Constant, true);
    }
    
    AtomDestiny::ObjectPool::Instance().Despawn(scrap, m_partsDestroyTime);
}
