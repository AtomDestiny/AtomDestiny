#include "UnitLogicBase.h"

#include <algorithm>

#include <AtomDestiny/Core/MathUtils.h>
#include <AtomDestiny/Core/ActorUtils.h>

#include "AtomDestiny/Core/ActorComponentUtils.h"

const TArray<TScriptInterface<IWeapon>>& UUnitLogicBase::GetAllWeapon() const
{
    return m_weapons;
}

EUnitSize UUnitLogicBase::GetSize() const
{
    return m_unitSize;
}

EGameSide UUnitLogicBase::GetSide() const
{
    return m_side;
}

int32 UUnitLogicBase::GetCost() const
{
    return m_unitCost;
}

EUnitType UUnitLogicBase::GetUnitType() const
{
    return m_unitType;
}

double UUnitLogicBase::GetVelocity() const
{
    return m_currentSpeed;
}

void UUnitLogicBase::SetSide(EGameSide side)
{
    m_side = side;
}

void UUnitLogicBase::InitializeComponent()
{
    Super::InitializeComponent();
    
    // get all weapons
    m_weapons = GET_INTERFACES(Weapon);

    // get navigation
    APawn* pawn = CastChecked<APawn>(GetOwner());
    check(pawn->AIControllerClass != nullptr);
    
    m_navigation = CastChecked<ANavigator>(pawn->AIControllerClass);
    m_navigation->SetMovementComponent(pawn->FindComponentByClass<UFloatingPawnMovement>());
    
    m_speed = m_navigation->GetSpeed();
    m_currentSpeed = m_speed;

    CalculateDistances();

    AddNewParameter(EObjectParameters::Velocity);
}

void UUnitLogicBase::BeginPlay()
{
    Super::BeginPlay();
    
    // navigation setup
    m_defaultStopDistance = m_navigation->GetStopDistance();

    // get animation
    m_animation = GET_INTERFACE(Animation);
    
    // setup scan delay
    m_scanDelay += FMath::RandRange(AtomDestiny::Unit::MinRandomScan, AtomDestiny::Unit::MaxRandomScan);

    // new layer
    // Utils.SetLayerRecursively(gameObject, LayerMask.NameToLayer(Core.GetLayerNameFromSide(side)));
    CreateMessage();
}

void UUnitLogicBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);
    DestroyMessage();
}

void UUnitLogicBase::RotateToTarget(float deltaTime)
{
    if (!m_currentDestination.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Destination for unit is invalid"));
        return;
    }

    // calculate vector on target
    const FVector weaponLocation = GetOwner()->GetTransform().GetLocation();
    FVector targetVector = m_currentDestination->GetTransform().GetLocation() - weaponLocation;
    targetVector.Y = 0;

    // angle between unit and target
    const auto angle = AtomDestiny::Vector::Angle(targetVector, weaponLocation.ForwardVector);
    m_isRotatedOnTarget = (FMath::Abs(angle) < m_attackAngle);

    // TODO: check to swap last parameters
    const FVector lookVector = FMath::VInterpNormalRotationTo(weaponLocation.ForwardVector, targetVector,
                                                              m_rotateSpeed * deltaTime, 0.1f);
    const FQuat lookRotation = FQuat::FindBetween(lookVector, FVector::UpVector);

    GetOwner()->SetActorRotation(lookRotation);
}

FAsyncCoroutine UUnitLogicBase::StartScanDelay()
{
    m_canScan = false;
    co_await UE5Coro::Latent::Seconds(m_scanDelay);
    m_canScan = true;
}

void UUnitLogicBase::CheckScanDelay(float deltaTime)
{
    if (!m_canScan)
    {
        m_scanDelayCounter += deltaTime;

        if (m_scanDelayCounter >= m_scanDelay)
        {
            m_scanDelayCounter = 0;
            m_canScan = true;
        }
    }
}

void UUnitLogicBase::CalculateDistances()
{
    ///
    /// This was made specially, because TScriptInterface does not have operator=()
    /// so this boilerplate code is needed.
    /// Complexity of algorithm is not a problem, weapon counts are around 2.
    /// Do not use std::max_element/std::min_element/std::transform on weapons.
    ///

    std::vector<double> attackRanges;
    attackRanges.reserve(m_weapons.Num());

    for (const TScriptInterface<IWeapon>& weapon : m_weapons)
        attackRanges.push_back(weapon->GetBaseAttackRange());

    // scan setup
    const auto maxIter = std::max_element(std::begin(attackRanges), std::end(attackRanges));
    const double maxRange = maxIter != std::end(attackRanges) ? *maxIter : 0;
    m_scanDistance = maxRange + m_attackDeltaRange;

    attackRanges.clear();

    for (const TScriptInterface<IWeapon>& weapon : m_weapons)
        attackRanges.push_back(weapon->GetMinAttackRange());

    // min scan setup
    const auto minIter = std::min_element(std::begin(attackRanges), std::end(attackRanges));
    m_minScanDistance = minIter != std::end(attackRanges) ? *minIter : 0;
}

void UUnitLogicBase::RecalculateParameter(EObjectParameters parameter)
{
    if (!GetParameterAvailable(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("You try to recalculate invalid paramter at UnitLogic"));
        return;
    }

    switch (parameter)
    {
    case EObjectParameters::Velocity:
        {
            m_currentSpeed = CalculateParametersFromAll(m_speed, parameter);
            m_navigation->SetSpeed(m_currentSpeed);

            break;
        }

    default:
        break;
    }
}

void UUnitLogicBase::ZeroizeParameter(EObjectParameters parameter)
{
    switch (parameter)
    {
    case EObjectParameters::Velocity:
        m_currentSpeed = 0;
        m_navigation->SetSpeed(m_currentSpeed);
        break;

    default:
        break;
    }
}

void UUnitLogicBase::CreateMessage() const
{
    unitCreated.Broadcast(GetOwner(), m_side, m_unitType);
}

void UUnitLogicBase::DestroyMessage() const
{
    unitDestroyed.Broadcast(GetOwner(), m_side, m_unitType);
}
