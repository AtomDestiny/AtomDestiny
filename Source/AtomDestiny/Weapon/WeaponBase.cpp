#include "WeaponBase.h"

#include <AtomDestiny/Core/MathUtils.h>

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
        const TStrongObjectPtr ptr { m_projectileBlueprint };
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
    
    m_weaponAnimation = TObjectPtr<IWeaponAnimation> { GetOwner()->FindComponentByClass<IWeaponAnimation>() };

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

void UWeaponBase::RotateToTarget(float deltaTime)
{
    if (!m_target.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Target for weapon is invalid"));
        return;
    }

    // calculate vector on target
    const auto weaponLocation = m_weaponTransform->GetComponentTransform().GetLocation();
    FVector targetVector = m_target->GetTransform().GetLocation() - weaponLocation;
    targetVector.Y = 0;
    
    // angle between unit and target
    const auto angle = AtomDestiny::Vector::Angle(targetVector, weaponLocation.ForwardVector);
    m_isRotatedOnTarget = (FMath::Abs(angle) < m_attackAngle);

    // TODO: check to swap last parameters
    const auto lookVector = FMath::VInterpNormalRotationTo(weaponLocation.ForwardVector, targetVector, m_rotateSpeed * deltaTime, 0.1f);
    const auto lookRotation = FQuat::FindBetween(lookVector, FVector::UpVector);
    
    m_weaponTransform->SetWorldRotation(lookRotation);
}
