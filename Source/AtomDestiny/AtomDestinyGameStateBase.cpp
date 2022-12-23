#include "AtomDestinyGameStateBase.h"

#include <unordered_set>
#include <unordered_map>

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Core/Hash.h>
#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include <Engine/Classes/GameFramework/Actor.h>

#include "Core/ActorComponentUtils.h"

namespace
{
    constexpr double MinimalCriticalRange = 0;
    constexpr double MaximumCriticalRange = 100;
    
    FVector GetImpactPoint(const TScriptInterface<IProjectile>& projectile, EProjectileDamageOptions options)
    {
        switch (options)
        {
        case EProjectileDamageOptions::ProjectilePoint:
            return CastChecked<AActor>(projectile.GetInterface())->GetActorLocation();

        case EProjectileDamageOptions::ImpactPoint:
            return projectile->GetPoints().impactPosition;

        case EProjectileDamageOptions::TargetPoint:
            return projectile->GetParameters().target->GetActorLocation();
            
        default:
            return FVector{};
        }
    }

    // calculates critical damage if it is possible
    double CalculatePossibleCriticalDamage(double criticalChance, double criticalRate, double damage)
    {
        if (const double generatedValue = FMath::RandRange(MinimalCriticalRange, MaximumCriticalRange); generatedValue < criticalChance)
            damage *= criticalRate;

        return damage;
    }
    
} // namespace

// Place core data here
struct AAtomDestinyGameStateBase::GameStateBasePrivateData
{
    // represents active units at overall battle
    TMap<EGameSide, FSharedGameStateUnitList> activeUnits;

    // represents references to Active units, that could be threaded as enemies
    TMap<EGameSide, FEnemiesList> enemies;
};

AAtomDestinyGameStateBase::AAtomDestinyGameStateBase() :
    m_impl(new GameStateBasePrivateData{})
{
    InitializeSides();
    InitializeEnemies();
    
    UUnitLogicBase::unitCreated.AddDynamic(this, &AAtomDestinyGameStateBase::OnUnitCreated);
    UUnitLogicBase::unitDestroyed.AddDynamic(this, &AAtomDestinyGameStateBase::OnUnitDestroyed);
}

AAtomDestinyGameStateBase::~AAtomDestinyGameStateBase()
{
    UUnitLogicBase::unitCreated.RemoveDynamic(this, &AAtomDestinyGameStateBase::OnUnitCreated);
    UUnitLogicBase::unitDestroyed.RemoveDynamic(this, &AAtomDestinyGameStateBase::OnUnitDestroyed);
    
    delete m_impl;
}

void AAtomDestinyGameStateBase::AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    if (!actor.IsValid())
    {
        LOG_WARNING(TEXT("Trying to add invalid unit to game state"));
        return;
    }

    const FSharedGameStateUnitList& unitList = m_impl->activeUnits[side];

    if (const auto index = unitList->Find(actor); index == INDEX_NONE)
        unitList->Add(std::move(actor));
}

void AAtomDestinyGameStateBase::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    if (!actor.IsValid())
    {
        LOG_WARNING(TEXT("Trying to remove invalid unit from game state"));
        return;
    }
    
    const FSharedGameStateUnitList& unitList = m_impl->activeUnits[side];

    if (const auto index = unitList->Find(actor); index != INDEX_NONE)
        unitList->RemoveAt(index);
}

TWeakObjectPtr<AActor> AAtomDestinyGameStateBase::GetDestination(EGameSide side) const
{
    switch (side)
    {
    case EGameSide::Rebels:
        return m_destination.playerDestination;

    case EGameSide::Federation:
        return m_destination.enemyDestination;

    case EGameSide::Neutral:
        return m_destination.neutralDestination;

    default:
        return nullptr;
    }
}

const FEnemiesList& AAtomDestinyGameStateBase::GetEnemies(EGameSide side) const
{
    return m_impl->enemies[side];
}

void AAtomDestinyGameStateBase::AddDamage(const TScriptInterface<IProjectile>& projectile, EProjectileDamageOptions options)
{
    if (projectile == nullptr)
        return;
    
    const FWeaponParameters& weaponParameters = projectile->GetParameters();

    if (weaponParameters.explosionRadius > 0)
    {
        const FVector impactPoint = GetImpactPoint(projectile, options);
        const UWorld* world = projectile->GetParameters().owner->GetWorld();

        TArray<FOverlapResult> collisionResult;
        FCollisionShape sphere;
        sphere.SetSphere(weaponParameters.explosionRadius);
        
        if (world->OverlapMultiByObjectType(collisionResult, impactPoint, FQuat::Identity, FCollisionObjectQueryParams::DefaultObjectQueryParam, sphere))
        {
            std::unordered_set<AActor*> filteredActors;

            for (const FOverlapResult& overlapResult : collisionResult)
            {
                AActor* actor = overlapResult.GetActor();
                
                if (!filteredActors.contains(actor))
                {
                    filteredActors.insert(actor);

                    if (const TScriptInterface<IObjectState> objectState = GET_ACTOR_INTERFACE(ObjectState, actor); objectState != nullptr)
                        AddDamageToState(objectState, weaponParameters);
                }
            }
        }
    }
    else
    {
        if (const TScriptInterface<IObjectState> objectState = GET_ACTOR_INTERFACE(ObjectState, weaponParameters.target.Get()); objectState != nullptr)
            AddDamageToState(objectState, weaponParameters);
    }
}

void AAtomDestinyGameStateBase::AddDamageToState(const TScriptInterface<IObjectState>& objectState, const FWeaponParameters& parameters)
{
    double resultDamage = parameters.damage;

    if (parameters.criticalChance > 0 && parameters.criticalRate > 1)
    {
        resultDamage = CalculatePossibleCriticalDamage(parameters.criticalChance, parameters.criticalRate, resultDamage);
    }

    if (objectState != nullptr)
        objectState->AddDamage(resultDamage, parameters.weaponType, parameters.owner.Get());
}

void AAtomDestinyGameStateBase::OnUnitCreated(AActor* actor, EGameSide side, EUnitType)
{
    AddUnit(MakeWeakObjectPtr(actor), side);
}

void AAtomDestinyGameStateBase::OnUnitDestroyed(AActor* actor, EGameSide side, EUnitType)
{
    RemoveUnit(MakeWeakObjectPtr(actor), side);
}

void AAtomDestinyGameStateBase::InitializeSides()
{
    constexpr auto lastSide = static_cast<uint8>(EGameSide::None);

    for (uint8 side = 0; side < lastSide; ++side)
    {
        if (const EGameSide currentSide = static_cast<EGameSide>(side); !m_impl->activeUnits.Contains(currentSide))
        {
            TSharedPtr<FGameStateUnitList> ptr { new FGameStateUnitList() };
            m_impl->activeUnits.Add(currentSide, std::move(ptr));
        }
    }
}

void AAtomDestinyGameStateBase::InitializeEnemies()
{
    for (const auto& [side, list] : m_impl->activeUnits)
    {
        for (const auto& [s, l] : m_impl->activeUnits)
        {
            if (side != s && side != EGameSide::None)
                m_impl->enemies[side].Add(m_impl->activeUnits[s]);
        }
    }
}
