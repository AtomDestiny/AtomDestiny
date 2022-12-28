#include "UnitLogic.h"

#include <limits>
#include <AtomDestiny/AtomDestinyGameStateBase.h>
#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/Utils.h>

namespace
{
    constexpr double MaxScanDistance = std::numeric_limits<double>::max();
    
} // namespace

UUnitLogic::UUnitLogic(const FObjectInitializer& objectInitializer):
    UUnitLogicBase(objectInitializer)
{
    SetTickEnabled(true);
}

void UUnitLogic::UpdateParameters()
{
    CalculateDistances();
}

void UUnitLogic::SetDestination(AActor* destination)
{
    m_currentDestination = nullptr;
    m_canScan = true;
    
    m_behaviour = destination != nullptr ? EUnitBehaviour::MoveToTransform : EUnitBehaviour::Standing;
    m_mainDestination = destination != nullptr ? MakeWeakObjectPtr(destination) : nullptr;
}

void UUnitLogic::SetDestinationByPoint(const FVector& destination)
{
    m_currentDestination = nullptr;
    m_mainDestination = nullptr;
    m_canScan = true;
    
    m_behaviour = EUnitBehaviour::MoveToPoint;
    m_destinationPoint = destination;
}

void UUnitLogic::BeginPlay()
{
    Super::BeginPlay();
    
    CreateDestination();
}

void UUnitLogic::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* func)
{
    Super::TickComponent(deltaTime, tickType, func);

    CheckTargetDistance();
    CheckNavigation();
    CheckScanDelay(deltaTime);

    if (!m_isAttacking && m_canScan)
    {
        ScanEnemy();
    }

    if (m_isTargetFound && !m_isAttacking)
    {
        UpdateNavigationTarget();
    }

    if (m_isTargetFound)
    {
        TryToAttack(deltaTime);
    }
    else
    {
        SetDefaultDestination();
    }
}

void UUnitLogic::CheckTargetDistance()
{
    if (m_currentDestination.IsValid() && m_isAttacking)
    {
        const double scanLengthSquared = m_scanDistance * m_scanDistance;
        const FVector targetVector = m_currentDestination->GetActorLocation() - GetOwner()->GetActorLocation();
        
        if (targetVector.SquaredLength() <= (scanLengthSquared + m_tryAttackDelta))
        {
            return;
        }
        
        m_isAttacking = false;
        m_isTargetFound = false;
        m_currentDestination = nullptr;

        for (const TScriptInterface<IWeapon>& weapon : m_weapons)
        {
            if (weapon != nullptr)
            {
                weapon->SetTarget(nullptr);
            }
        }
    }
}

void UUnitLogic::CreateDestination()
{
    if (!m_navigation.IsValid())
    {
        return;
    }

    const TWeakObjectPtr<AActor> destination = AtomDestiny::GetGameState(GetOwner())->GetDestination(m_side);
    
    if (destination.Get() && m_behaviour == EUnitBehaviour::MoveToTransform)
    {
        m_mainDestination = destination;
        m_currentDestination = m_mainDestination;
        
        m_navigation->Move(m_currentDestination->GetActorLocation());
    }
    else if (m_behaviour == EUnitBehaviour::MoveToPoint)
    {
        m_navigation->Move(m_destinationPoint);
    }
    else if (m_behaviour == EUnitBehaviour::Standing)
    {
        m_navigation->Move(GetOwner()->GetActorLocation());
        m_navigation->Stop();
    }
    else
    {
        MoveNearestEnemyIfCan();
    }
}

void UUnitLogic::CheckNavigation()
{
    if (!m_currentDestination.IsValid())
    {
        SetDefaultDestination();

        m_isTargetFound = false;
        m_isAttacking = false;
    }
    else if ((m_currentDestination->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength() <= (m_minScanDistance * m_minScanDistance))
    {
        for (const TScriptInterface<IWeapon>& weapon : m_weapons)
        {
            weapon.GetInterface()->SetTarget(nullptr);
        }
        
        SetDefaultDestination();

        m_isTargetFound = false;
        m_isAttacking = false;
    }
}

void UUnitLogic::SetDefaultDestination()
{
    if (m_behaviour == EUnitBehaviour::MoveToPoint)
    {
        m_navigation->Move(m_destinationPoint);
    }
    else
    {
        if (m_mainDestination.Get() && m_behaviour == EUnitBehaviour::MoveToTransform)
        {
            m_currentDestination = m_mainDestination;
            m_navigation->Move(m_currentDestination->GetActorLocation());
        }
        else
        {
            MoveNearestEnemyIfCan();
        }
    }

    m_navigation->SetStopDistance(m_defaultStopDistance);

    if (m_navigation->GetRemainingDistance() > m_navigation->GetStopDistance())
    {
        if (m_animation != nullptr)
        {
            m_animation->Walk();
        }
    }
}

void UUnitLogic::UpdateNavigationTarget()
{
    if (m_currentDestination.Get() && m_behaviour == EUnitBehaviour::MoveToTransform)
    {
        m_navigation->Move(m_currentDestination->GetActorLocation());
    }
    else if (m_behaviour == EUnitBehaviour::MoveToPoint)
    {
        m_navigation->Move(m_destinationPoint);
    }
    else if (m_behaviour == EUnitBehaviour::Standing && m_mainDestination.Get() && m_currentDestination.Get())
    {
        m_navigation->Move(m_currentDestination->GetActorLocation());
    }

    m_navigation->SetStopDistance(m_defaultStopDistance);

    if (m_animation != nullptr)
    {
        m_animation->Walk();
    }
}

void UUnitLogic::MoveNearestEnemyIfCan()
{
    // So we have no destination and should try to search any possible enemy target
    if (const TWeakObjectPtr<AActor> target = FindEnemy(0, std::numeric_limits<double>::max()); target.IsValid())
    {
        m_navigation->Move(target.Get());
        m_currentDestination = std::move(target);
    }
    else
    {
        m_navigation->Move(GetOwner());
    }
}

void UUnitLogic::ScanEnemy()
{
    TWeakObjectPtr<AActor> target = FindEnemy(m_minScanDistance, m_scanDistance);

    if (target.IsValid())
    {
        m_currentDestination = std::move(target);
        m_isTargetFound = true;
    }

    m_canScan = false;
}

void UUnitLogic::TryToAttack(float deltaTime)
{
    m_isAttacking = false;
        
    if (m_currentDestination != nullptr)
    {
        const FVector targetVector = m_currentDestination->GetActorLocation() - GetOwner()->GetActorLocation();
        
        for (TScriptInterface<IWeapon>& weapon : m_weapons)
        {      
            weapon->SetTarget(m_currentDestination.Get());
        
            auto weaponSqrDist = weapon->GetAttackRange();
            weaponSqrDist *= weaponSqrDist;
        
            if (targetVector.SquaredLength() <= weaponSqrDist + m_tryAttackDelta)
            {
                if (weapon->IsSeeTarget())
                {
                    Aim(weapon, deltaTime);
                }
            }
        }
    }
}

void UUnitLogic::Aim(const TScriptInterface<IWeapon>& weapon, float deltaTime)
{
    m_isAttacking = true;
        
    if (weapon->IsRotated())
    {
        CheckBehaviour(weapon);
        weapon->Fire(deltaTime);
    }
    else
    {
        m_navigation->Stop();

        if (m_animation != nullptr)
        {
            m_animation->Idle();
        }
        
        RotateToTarget(deltaTime);
        
        if (m_isRotatedOnTarget)
        {
            CheckBehaviour(weapon);
            weapon->Fire(deltaTime);
        }
        else
        {
            m_isAttacking = false;
        }
    }
}

void UUnitLogic::CheckBehaviour(const TScriptInterface<IWeapon>& weapon)
{
    if (weapon->IsShouldStop())
    {
        m_navigation->Stop();
        
        if (m_animation != nullptr)
        {
            m_animation->Attack();
        }
    }
}

// Search algorithm optimized to used squared length
TWeakObjectPtr<AActor> UUnitLogic::FindEnemy(double minScanDistance, double scanDistance) const
{
    const TWeakObjectPtr<AAtomDestinyGameStateBase> gameState = AtomDestiny::GetGameState(GetOwner());

    if (!gameState.IsValid() || !gameState->IsEnemiesExist(m_side) || gameState->GetEnemies(m_side).IsEmpty())
    {
        return nullptr;
    }
        
    const FEnemiesList& enemies = gameState->GetEnemies(m_side);
    double minDist = ::MaxScanDistance;
    AActor* enemy = nullptr;
    
    const double sqrScanDistance = scanDistance * scanDistance;
    const double sqrMinScanDistance = minScanDistance * minScanDistance;
    const size_t enemyListSideCount = static_cast<size_t>(enemies.Num());

    for (size_t sideCount = 0; sideCount < enemyListSideCount; ++sideCount)
    {
        const size_t enemyListUnitCount = static_cast<size_t>(enemies[sideCount]->Num());

        for (int unitCount = 0; unitCount < enemyListUnitCount; ++unitCount)
        {
            if (AActor* target = (*enemies[sideCount])[unitCount].Get(); target != nullptr)
            {
                const double sqrMagnitude = (target->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();

                if ((sqrScanDistance >= sqrMagnitude) && (sqrMinScanDistance <= sqrMagnitude))
                {
                    if (sqrMagnitude <= minDist)
                    {
                        minDist = sqrMagnitude;
                        enemy = target;
                    }
                }
            }
        }
    }

    return MakeWeakObjectPtr(enemy);
}
