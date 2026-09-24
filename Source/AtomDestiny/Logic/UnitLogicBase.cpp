#include "UnitLogicBase.h"

#include <algorithm>

#include <AtomDestiny/AtomDestinyGameStateBase.h>
#include <AtomDestiny/Core/MathUtils.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/Logger.h>

#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"

UUnitLogicBase::UUnitLogicBase(const FObjectInitializer& objectInitializer):
    UADObject(objectInitializer)
{
    // Units placed on a level start logic on their own
    bAutoActivate = true;
}

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

EADUnitType UUnitLogicBase::GetUnitType() const
{
    return m_unitType;
}

double UUnitLogicBase::GetVelocity() const
{
    return m_currentSpeed;
}

void UUnitLogicBase::SetSide(EGameSide side)
{
    if (m_side == side)
        return;

    const EGameSide oldSide = m_side;
    m_side = side;

    if (m_isRegistered)
        unitSideChanged.Broadcast(GetOwner(), oldSide, side);
}

void UUnitLogicBase::Activate(bool bReset)
{
    Super::Activate(bReset);

    // Auto activation comes before BeginPlay, logic starts there
    if (HasBegunPlay())
        StartLogic();
}

void UUnitLogicBase::Deactivate()
{
    if (HasBegunPlay())
        StopLogic();

    Super::Deactivate();
}

void UUnitLogicBase::InitializeComponent()
{
    Super::InitializeComponent();

    m_weapons = AtomDestiny::Utils::GetInterfaces<IWeapon, UWeapon>(GetOwner());

    CalculateDistances();

    AddNewParameter(EObjectParameters::Velocity);
}

void UUnitLogicBase::BeginPlay()
{
    Super::BeginPlay();

    m_animation = AtomDestiny::Utils::GetInterface<IAnimation>(GetOwner());
    m_scanDelay += FMath::RandRange(AtomDestiny::Unit::MinRandomScan, AtomDestiny::Unit::MaxRandomScan);

    if (IsActive())
        StartLogic();
    else
        SetComponentTickEnabled(false);
}

void UUnitLogicBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);
    UnregisterFromGameState();
}

void UUnitLogicBase::RotateToTarget(float deltaTime)
{
    if (!m_currentDestination.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Destination for unit is invalid"));
        return;
    }

    const auto [angle, rotation] = AtomDestiny::LerpRotation(GetOwner(), m_currentDestination.Get(), deltaTime, m_rotateSpeed);

    m_isRotatedOnTarget = (FMath::Abs(angle) < m_attackAngle);
    GetOwner()->SetActorRotation(rotation);
}

void UUnitLogicBase::CheckScanDelay(float deltaTime)
{
    if (!m_canScan)
    {
        m_scanDelayCounter += static_cast<double>(deltaTime);

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

            if (m_navigation.IsValid())
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

        if (m_navigation.IsValid())
            m_navigation->SetSpeed(m_currentSpeed);
        break;

    default:
        break;
    }
}

void UUnitLogicBase::CreateEvent() const
{
    unitCreated.Broadcast(GetOwner(), m_side, m_unitType);
}

void UUnitLogicBase::DestroyEvent() const
{
    unitDestroyed.Broadcast(GetOwner(), m_side, m_unitType);
}

void UUnitLogicBase::RegisterInGameState()
{
    if (m_isRegistered)
        return;

    m_isRegistered = true;
    CreateEvent();
}

void UUnitLogicBase::UnregisterFromGameState()
{
    if (!m_isRegistered)
        return;

    m_isRegistered = false;
    DestroyEvent();
}

bool UUnitLogicBase::InitNavigation()
{
    if (m_navigation.IsValid())
        return true;

    APawn* pawn = CastChecked<APawn>(GetOwner());
    check(pawn->AIControllerClass != nullptr);

    if (pawn->Controller == nullptr)
        pawn->SpawnDefaultController();

    ANavigator* navigator = Cast<ANavigator>(pawn->Controller.Get());
    if (navigator == nullptr)
    {
        LOG_ERROR(TEXT("Pawn AIControllerClass should be an ANavigator or derived from"));
        return false;
    }

    if (navigator->GetPawn() != pawn)
        navigator->Possess(pawn);

    const auto movement = pawn->FindComponentByClass<UFloatingPawnMovement>();

    m_navigation = MakeWeakObjectPtr(navigator);
    m_navigation->SetMovementComponent(movement);
    m_navigation->AttachToActor(pawn, FAttachmentTransformRules::KeepRelativeTransform);
    m_navigation->SetPawn(pawn);
    m_navigation->AActor::SetActorLocation(pawn->GetActorLocation());

    m_speed = m_navigation->GetSpeed();

    if (m_speed <= 0.0 && movement != nullptr)
        m_speed = movement->MaxSpeed;

    m_navigation->SetStopDistance(m_defaultStopDistance);
    return true;
}

void UUnitLogicBase::StartLogic()
{
    if (!InitNavigation())
    {
        SetComponentTickEnabled(false);
        return;
    }

    m_currentSpeed = m_speed;
    m_navigation->SetSpeed(m_currentSpeed);

    RegisterInGameState();
}

void UUnitLogicBase::StopLogic()
{
    UnregisterFromGameState();

    if (m_navigation.IsValid())
        m_navigation->Stop();

    for (const TScriptInterface<IWeapon>& weapon : m_weapons)
    {
        if (weapon != nullptr)
            weapon->SetTarget(nullptr);
    }

    if (m_animation != nullptr)
        m_animation->Idle();

    ClearParameterEnhancements();

    m_mainDestination = nullptr;
    m_currentDestination = nullptr;
    m_isTargetFound = false;
    m_isAttacking = false;
    m_isRotatedOnTarget = false;
    m_canScan = true;
    m_scanDelayCounter = 0;
    m_behaviour = EUnitBehaviour::MoveToTransform;
    m_destinationPoint = FVector::ZeroVector;
}

#if !UE_BUILD_SHIPPING
bool UUnitLogicBase::TryGetNavigationGoalLocation(FVector& outWorldLocation) const
{
    if (m_currentDestination.IsValid())
    {
        outWorldLocation = m_currentDestination->GetActorLocation();
        return true;
    }

    if (m_behaviour == EUnitBehaviour::MoveToPoint)
    {
        outWorldLocation = m_destinationPoint;
        return true;
    }

    if (m_mainDestination.IsValid())
    {
        outWorldLocation = m_mainDestination->GetActorLocation();
        return true;
    }

    if (m_behaviour == EUnitBehaviour::MoveToTransform)
    {
        if (const TWeakObjectPtr<AAtomDestinyGameStateBase> gameState = AtomDestiny::GetGameState(GetOwner());
            gameState.IsValid())
        {
            if (const AActor* rallyPoint = gameState->GetRallyPoint(m_side);
                rallyPoint != nullptr)
            {
                outWorldLocation = rallyPoint->GetActorLocation();
                return true;
            }
        }
    }

    return false;
}
#endif
