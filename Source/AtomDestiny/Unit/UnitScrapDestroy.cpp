#include "UnitScrapDestroy.h"

#include "AtomDestiny/Core/ObjectPool/ActorPool.h"
#include "AtomDestiny/Core/ActorComponentUtils.h"
#include "AtomDestiny/Core/Logger.h"
#include "AtomDestiny/Logic/Logic.h"
#include "AtomDestiny/Unit/UnitTeamColor.h"

#include "AtomDestiny/Misc/ScrapConstruction.h"

#include <Components/StaticMeshComponent.h>

UUnitScrapDestroy::UUnitScrapDestroy(const FObjectInitializer& objectInitializer):
    UDestroyBase(objectInitializer)
{
}

void UUnitScrapDestroy::BeginPlay()
{
    Super::BeginPlay();

    AtomDestiny::ObjectPool::Instance().Preload(m_scrapPrefab, ScrapPreloadCount);
}

void UUnitScrapDestroy::Destroy()
{
    if (m_destroyed)
    {
        return;
    }

    const FVector actorLocation = GetOwner()->GetActorLocation();
    const FQuat actorRotation = GetOwner()->GetActorQuat();

    const TScriptInterface<ILogic> logic = AtomDestiny::Utils::GetInterface<ILogic>(GetOwner());
    const EGameSide side = logic != nullptr ? logic->GetSide() : EGameSide::None;

    Super::Destroy();
    SpawnExplosion(actorLocation, FQuat::Identity);

    AtomDestiny::ObjectPool::Instance().Despawn(GetOwner());

    if (!IsValid(m_scrapPrefab))
    {
        LOG_ERROR(TEXT("Scrap prefab is empty"));
        return;
    }

    const TWeakObjectPtr<AActor> scrap = AtomDestiny::ObjectPool::Instance().Spawn(m_scrapPrefab.GetDefaultObject(), actorLocation, actorRotation);

    // Scrap parts share team colored material with the unit
    AtomDestiny::TeamColor::Apply(scrap.Get(), side);

    const TArray<UStaticMeshComponent*> components = AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(scrap.Get());

    for (const auto component : components)
    {
        const double power = FMath::FRandRange(m_minExplosionPower, m_maxExplosionPower);

        component->SetSimulatePhysics(true);
        component->AddRadialImpulse(scrap->GetActorLocation(), m_explosionRadius, static_cast<float>(power), ERadialImpulseFalloff::RIF_Constant, true);
    }

    if (UScrapConstruction* construction = scrap->FindComponentByClass<UScrapConstruction>(); construction != nullptr)
    {
        construction->Construct();
    }

    AtomDestiny::ObjectPool::Instance().Despawn(scrap, m_partsDestroyTime);
}
