#include "WeaponBase.h"

#include <AtomDestiny/Logic/Logic.h>

#include <AtomDestiny/Core/MathUtils.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

void UWeaponBase::InitializeComponent()
{
    Super::InitializeComponent();
    
    m_weaponTransform = this;
        
    m_currentDamage = m_damage;
    m_currentAttackRange = m_attackRange;
    m_currentExplosionRadius = m_explosionRadius;
    m_currentCriticalChance = m_criticalChance;
    m_currentCriticalRate = m_criticalRate;
    m_currentReloadTime = m_reloadTime;

    m_minShotSqrtDistance = m_minShotDistance * m_minShotDistance;

    if (m_projectileBlueprint.IsValid())
    {
        const TStrongObjectPtr ptr { m_projectileBlueprint.Get() };
        AtomDestiny::ObjectPool::Instance().Preload(ptr, BlueprintPreloadCount);
    }

    const std::vector params = {
        EObjectParameters::Reload,
        EObjectParameters::ExplosionRadius,
        EObjectParameters::Damage,
        EObjectParameters::CriticalRate,
        EObjectParameters::CriticalChance,
        EObjectParameters::Range
    };
        
    AddNewParameters(params);
}

void UWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    
    m_weaponAnimation = AtomDestiny::Utils::GetInterface<IWeaponAnimation, UWeaponAnimation>(GetOwner());

    if (m_useRaycast && !m_useFriendlyFire)
        ExcludeSameLayer();
}

void UWeaponBase::EndPlay(const EEndPlayReason::Type type)
{
    Super::EndPlay(type);
    m_firing = false;
    // StopAllCoroutines();
}

void UWeaponBase::Activate(bool state)
{
    Super::Activate(state);
    m_firing = false;
    // StopAllCoroutines();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UWeaponBase::ExcludeSameLayer()
{
    // raycastMask = raycastMask.RemoveFromMask(gameObject.layer);
}

bool UWeaponBase::IsFiring() const
{
    return m_firing;
}

bool UWeaponBase::IsAoe() const
{
    return m_explosionRadius > 0;
}

double UWeaponBase::GetDamage() const
{
    return m_currentDamage;
}

bool UWeaponBase::IsCritical() const
{
    return (m_criticalChance > 0 && m_criticalRate > 1);
}

bool UWeaponBase::IsShouldStop() const
{
    return m_stopWhenAttack;
}

bool UWeaponBase::IsRaycasted() const
{
    return m_useRaycast;
}

bool UWeaponBase::IsFriendlyFireAvailable() const
{
    return m_useFriendlyFire;
}

double UWeaponBase::GetReloadTime() const
{
    return m_currentReloadTime;
}

double UWeaponBase::GetBaseReloadTime() const
{
    return m_reloadTime;
}

void UWeaponBase::SetTarget(AActor* target)
{
    m_target = TWeakObjectPtr<AActor> { target };
}

double UWeaponBase::GetAttackRange() const
{
    return m_currentAttackRange;
}

double UWeaponBase::GetMinAttackRange() const
{
    return m_minShotDistance;
}

bool UWeaponBase::IsRotated() const
{
    return m_rotatedWeapon;
}

EWeaponType UWeaponBase::GetWeaponType() const
{
    return m_weaponType;
}

double UWeaponBase::GetBaseDamage() const
{
    return m_damage;
}

double UWeaponBase::GetBaseAttackRange() const
{
    return m_attackRange;
}

double UWeaponBase::GetCriticalChance() const
{
    return m_currentCriticalChance;
}

double UWeaponBase::GetBaseCriticalChance() const
{
    return m_criticalChance;
}

double UWeaponBase::GetCriticalRate() const
{
    return m_currentCriticalRate;
}

double UWeaponBase::GetBaseCriticalRate() const
{
    return m_criticalRate;
}

FWeaponParameters UWeaponBase::GetParameters() const
{
    return FWeaponParameters {
        m_currentDamage,
        m_currentCriticalChance,
        m_currentCriticalRate,
        m_currentExplosionRadius,
        m_weaponType,
        this->GetOwner(),
        m_target
    };
}

void UWeaponBase::RotateToTarget(float deltaTime)
{
    if (!m_target.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Target for weapon is invalid"));
        return;
    }

    // calculate vector on target
    const FVector weaponLocation = m_weaponTransform->GetComponentTransform().GetLocation();
    FVector targetVector = m_target->GetTransform().GetLocation() - weaponLocation;
    targetVector.Y = 0;
    
    // angle between unit and target
    const auto angle = AtomDestiny::Vector::Angle(targetVector, weaponLocation.ForwardVector);
    m_isRotatedOnTarget = (FMath::Abs(angle) < m_attackAngle);

    // TODO: check to swap last parameters
    const FVector lookVector = FMath::VInterpNormalRotationTo(weaponLocation.ForwardVector, targetVector, m_rotateSpeed * deltaTime, 0.1f);
    const FQuat lookRotation = FQuat::FindBetween(lookVector, FVector::UpVector);
    
    m_weaponTransform->SetWorldRotation(lookRotation);
}

void UWeaponBase::RotateToRoot(float deltaTime) const
{
    if (!m_target.IsValid() && m_rotatedWeapon)
    {
        const FVector weaponLocation = m_weaponTransform->GetComponentTransform().GetLocation();
        const FVector rootLocation = GetOwner()->GetTransform().GetLocation();

        // TODO: check to swap last parameters
        const FVector lookVector = FMath::VInterpNormalRotationTo(weaponLocation.ForwardVector, rootLocation.ForwardVector, m_rotateSpeed * deltaTime, 0.1f);
        const FQuat lookRotation = FQuat::FindBetween(lookVector, FVector::UpVector);

        m_weaponTransform->SetWorldRotation(lookRotation);
    }
}

bool UWeaponBase::CheckRaycastToTarget(const FVector& origin, const FVector& direction) const
{
    // You can use this to customize various properties about the trace
    FCollisionQueryParams params;
    params.AddIgnoredActor(GetOwner());

    // The hit result gets populated by the line trace
    if (FHitResult hit; GetWorld()->LineTraceSingleByChannel(hit, origin, direction, m_layerMask, params))
        return hit.GetActor() == m_target;
    
    return false;
}

void UWeaponBase::FiringDelay()
{
    // yield return new WaitForSeconds(currentReloadTime);

    if (m_weaponTransform.IsValid())
        m_weaponAnimation->SetDefaultState();
        
    m_firing = false;
}

void UWeaponBase::RecalculateParameter(EObjectParameters parameter)
{
    const std::vector<FParameterEnhancement>& parameters = GetParameterEnhancementList(parameter);

    if (!GetParameterAvailable(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("U try to recalculate not available parameter"));
        return;
    }

    const auto calculator = [&parameters](const double startValue) {
        auto currentValue = startValue;

        for (const auto p : parameters)
            currentValue += InterpretParameterModifier(startValue, p);

        return currentValue;
    };
    
    switch (parameter)
    {
    case EObjectParameters::ExplosionRadius:
        m_currentExplosionRadius = calculator(m_explosionRadius);
        break;

    case EObjectParameters::Reload:
        m_currentReloadTime = calculator(m_reloadTime);
        break;

    case EObjectParameters::Damage:
        m_currentDamage = calculator(m_damage);
        break;

    case EObjectParameters::CriticalChance:
        m_currentCriticalChance = calculator(m_criticalChance);
        break;

    case EObjectParameters::CriticalRate:
        m_currentCriticalRate = calculator(m_criticalRate);
        break;

    case EObjectParameters::Range:
        {
            if (ILogic* logic = { AtomDestiny::Utils::GetInterface<ILogic, ULogic>(GetOwner()) }; logic != nullptr)
            {
                m_currentAttackRange = calculator(m_attackRange);
                logic->UpdateParameters();
            }
            
            break;
        }
    default:
        UE_LOG(LogTemp, Error, TEXT("Unavailable parameters to Recalculate"));
        break;
    }
}

void UWeaponBase::ZeroizeParameter(EObjectParameters parameter)
{
    switch (parameter)
    {
    case EObjectParameters::ExplosionRadius:
        m_currentExplosionRadius = 0;
        break;

    case EObjectParameters::Reload:
        m_currentReloadTime = 0;
        break;

    case EObjectParameters::Damage:
        m_currentDamage = 0;
        break;

    case EObjectParameters::CriticalChance:
        m_currentCriticalChance = 0;
        break;

    case EObjectParameters::CriticalRate:
        m_currentCriticalRate = 1;
        break;

    case EObjectParameters::Range:
        {
            if (ILogic* logic = { AtomDestiny::Utils::GetInterface<ILogic, ULogic>(GetOwner()) }; logic != nullptr)
            {
                m_currentAttackRange = 0;
                logic->UpdateParameters();
            }
            
            break;
        }

    default:
        UE_LOG(LogTemp, Error, TEXT("Unavailable parameters to Zeroize"));
        break;
    }
}
