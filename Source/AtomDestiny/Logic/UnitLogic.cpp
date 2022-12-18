#include "UnitLogic.h"

#include <limits>
#include <AtomDestiny/AtomDestinyGameStateBase.h>

namespace
{
    constexpr double MaxScanDistance = std::numeric_limits<double>::max();
    
} // namespace

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
    
    m_enemies = AtomDestiny::GetGameState(GetOwner())->GetEnemies(m_side);
}

void UUnitLogic::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* func)
{
    Super::TickComponent(deltaTime, tickType, func);

    CheckNavigation();
    CheckScanDelay(deltaTime);

    if (!m_isAttacking && m_canScan)
        ScanEnemy();

    if (m_isTargetFound && !m_isAttacking)
        UpdateNavigationTarget();

    if (m_isTargetFound)
        TryToAttack(deltaTime);
    else
        SetDefaultDestination();
}

void UUnitLogic::CreateDestination()
{
    const TWeakObjectPtr<AActor> destination = AtomDestiny::GetGameState(GetOwner())->GetDestination(m_side);

    if (destination.Get() && m_behaviour == EUnitBehaviour::MoveToTransform)
    {
        m_mainDestination = destination;
        m_currentDestination = m_mainDestination;
        
        m_navigation->Move(m_currentDestination->GetActorLocation());
    }
    else if (m_behaviour == EUnitBehaviour::MoveToPoint)
        m_navigation->Move(m_destinationPoint);
    else
        m_navigation->Move(GetOwner()->GetActorLocation());
}

void UUnitLogic::CheckNavigation()
{
    if (m_currentDestination == nullptr)
    {
        SetDefaultDestination();

        m_isTargetFound = false;
        m_isAttacking = false;
    }
    else if ((m_currentDestination->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength() <= (m_minScanDistance * m_minScanDistance))
    {
        for (const TScriptInterface<IWeapon>& weapon : m_weapons)
            weapon.GetInterface()->SetTarget(nullptr);
        
        SetDefaultDestination();

        m_isTargetFound = false;
        m_isAttacking = false;
    }
}

void UUnitLogic::SetDefaultDestination()
{
    if (m_behaviour == EUnitBehaviour::MoveToPoint)
        m_navigation->Move(m_destinationPoint);
    else
    {
        if (m_mainDestination.Get() && m_behaviour == EUnitBehaviour::MoveToTransform)
        {
            m_currentDestination = m_mainDestination;
            m_navigation->Move(m_currentDestination->GetActorLocation());
        }
        else
            m_navigation->Move(GetOwner()->GetActorLocation());
    }

    m_navigation->SetStopDistance(m_defaultStopDistance);

    if (m_navigation->GetRemainingDistance() > m_navigation->GetStopDistance())
    {
        if (m_animation != nullptr)
            m_animation->Walk();
    }
}

void UUnitLogic::UpdateNavigationTarget()
{
    if (m_currentDestination.Get() && m_behaviour == EUnitBehaviour::MoveToTransform)
        m_navigation->Move(m_currentDestination->GetActorLocation());
    else if (m_behaviour == EUnitBehaviour::MoveToPoint)
        m_navigation->Move(m_destinationPoint);
    else if (m_behaviour == EUnitBehaviour::Standing && m_mainDestination.Get() && m_currentDestination.Get())
        m_navigation->Move(m_currentDestination->GetActorLocation());

    m_navigation->SetStopDistance(m_defaultStopDistance);

    if (m_animation != nullptr)
        m_animation->Walk();
}

void UUnitLogic::ScanEnemy()
{
    double minDist = ::MaxScanDistance;

    const double sqrScanDistance = m_scanDistance * m_scanDistance;
    const double sqrMinScanDistance = m_minScanDistance * m_minScanDistance;
    const size_t enemyListSideCount = static_cast<size_t>(m_enemies->Num());

    for (size_t sideCount = 0; sideCount < enemyListSideCount; ++sideCount)
    {
        const size_t enemyListUnitCount = static_cast<size_t>((*m_enemies)[sideCount]->Num());

        for (int unitCount = 0; unitCount < enemyListUnitCount; ++unitCount)
        {
            if (AActor* target = (*(*m_enemies)[sideCount])[unitCount].Get(); target != nullptr)
            {
                const double sqrMagnitude = (target->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();

                if ((sqrScanDistance >= sqrMagnitude) && (sqrMinScanDistance <= sqrMagnitude))
                {
                    if (sqrMagnitude <= minDist)
                    {
                        m_currentDestination = MakeWeakObjectPtr(target);
                        minDist = sqrMagnitude;
                        m_isTargetFound = true;
                    }
                }
            }
        }
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
                    Aim(weapon, deltaTime);
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
            m_animation->Idle();
        
        RotateToTarget(deltaTime);
        
        if (m_isRotatedOnTarget)
        {
            CheckBehaviour(weapon);
            weapon->Fire(deltaTime);
        }
        else
            m_isAttacking = false;
    }
}

void UUnitLogic::CheckBehaviour(const TScriptInterface<IWeapon>& weapon)
{
    if (weapon->IsShouldStop())
    {
        m_navigation->Stop();
        
        if (m_animation != nullptr)
            m_animation->Attack();
    }
}
