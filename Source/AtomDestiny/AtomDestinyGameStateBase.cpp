#include "AtomDestinyGameStateBase.h"

#include <unordered_set>
#include <unordered_map>

// ReSharper disable once CppUnusedIncludeDirective
#include <AtomDestiny/Core/Hash.h>
#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Logic/UnitLogicBase.h>

#include <Engine/Classes/GameFramework/Actor.h>

#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

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
        {
            damage *= criticalRate;
        }

        return damage;
    }

    UWorld* GetWorldFromWeaponParameters(const FWeaponParameters& parameters)
    {
        return parameters.owner != nullptr ? parameters.owner->GetWorld() : GEngine->GetCurrentPlayWorld();
    }
    
} // namespace

AAtomDestinyGameStateBase::AAtomDestinyGameStateBase()
{
    InitializeSides();
    InitializeEnemies();
}

void AAtomDestinyGameStateBase::AddUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    if (!actor.IsValid())
    {
        LOG_WARNING(TEXT("Trying to add invalid unit to game state"));
        return;
    }
    
    const FSharedGameStateUnitList& unitListPtr = m_activeUnits[side];
    unitListPtr->AddUnique(actor);
}

void AAtomDestinyGameStateBase::RemoveUnit(TWeakObjectPtr<AActor> actor, EGameSide side)
{
    if (!actor.IsValid())
    {
        LOG_WARNING(TEXT("Trying to remove invalid unit from game state"));
        return;
    }
    
    const FSharedGameStateUnitList& unitListPtr = m_activeUnits[side];
    unitListPtr->Remove(actor);
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

bool AAtomDestinyGameStateBase::IsEnemiesExist(EGameSide side) const
{
    return m_enemies.Contains(side);
}

const FEnemiesList& AAtomDestinyGameStateBase::GetEnemies(EGameSide side) const
{
    return m_enemies[side];
}

void AAtomDestinyGameStateBase::AddDamage(const TScriptInterface<IProjectile>& projectile, EProjectileDamageOptions options)
{
    if (projectile == nullptr)
    {
        return;
    }
    
    const FWeaponParameters& weaponParameters = projectile->GetParameters();

    if (weaponParameters.explosionRadius > 0)
    {
        const FVector impactPoint = GetImpactPoint(projectile, options);
        const UWorld* world = GetWorldFromWeaponParameters(weaponParameters);

        TArray<FOverlapResult> collisionResult;
        FCollisionShape sphere;
        sphere.SetSphere(weaponParameters.explosionRadius);
        
        if (world->OverlapMultiByObjectType(collisionResult, impactPoint, FQuat::Identity, FCollisionObjectQueryParams::DefaultObjectQueryParam, sphere))
        {
            std::unordered_set<AActor*> filteredActors;

            for (const FOverlapResult& overlapResult : collisionResult)
            {
                if (AActor* actor = overlapResult.GetActor(); !filteredActors.contains(actor))
                {
                    filteredActors.insert(actor);

                    if (const TScriptInterface<IParameters> parameters = GET_ACTOR_INTERFACE(Parameters, actor); parameters != nullptr)
                    {
                        AddDamageToState(parameters, weaponParameters);
                    }
                }
            }
        }
    }
    else
    {
        if (const TScriptInterface<IParameters> parameters = GET_ACTOR_INTERFACE(Parameters, weaponParameters.target.Get()); parameters != nullptr)
        {
            AddDamageToState(parameters, weaponParameters);
        }
    }
}

void AAtomDestinyGameStateBase::AddDamageToState(const TScriptInterface<IParameters>& objectState, const FWeaponParameters& parameters)
{
    double resultDamage = parameters.damage;

    if (parameters.criticalChance > 0 && parameters.criticalRate > 1)
    {
        resultDamage = CalculatePossibleCriticalDamage(parameters.criticalChance, parameters.criticalRate, resultDamage);
    }

    if (objectState != nullptr)
    {
        objectState->AddDamage(resultDamage, parameters.weaponType, parameters.owner.Get());
    }
}

void AAtomDestinyGameStateBase::HandleBeginPlay()
{
    // specially added before parent call
    // Super::HandleBeginPlay will call BeginPlay for all actors
    UUnitLogicBase::unitCreated.AddDynamic(this, &AAtomDestinyGameStateBase::OnUnitCreated);
    UUnitLogicBase::unitDestroyed.AddDynamic(this, &AAtomDestinyGameStateBase::OnUnitDestroyed);
    
    Super::HandleBeginPlay();
}

void AAtomDestinyGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    UUnitLogicBase::unitCreated.RemoveDynamic(this, &AAtomDestinyGameStateBase::OnUnitCreated);
    UUnitLogicBase::unitDestroyed.RemoveDynamic(this, &AAtomDestinyGameStateBase::OnUnitDestroyed);

    m_activeUnits.Reset();
    m_enemies.Reset();

    AtomDestiny::ObjectPool::Instance().DestroyAll();
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
        if (const EGameSide currentSide = static_cast<EGameSide>(side); !m_activeUnits.Contains(currentSide))
        {
            TSharedPtr<FGameStateUnitList> ptr { new FGameStateUnitList() };
            m_activeUnits.Add(currentSide, std::move(ptr));
        }
    }
}

void AAtomDestinyGameStateBase::InitializeEnemies()
{
    for (const auto& [side, list] : m_activeUnits)
    {
        m_enemies.Add(side, FEnemiesList{});
        
        for (const auto& [s, l] : m_activeUnits)
        {
            if (side != s && side != EGameSide::None)
            {
                m_enemies[side].Add(m_activeUnits[s]);
            }
        }
    }
}
