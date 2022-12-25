#include "WeaponBase.h"

#include <AtomDestiny/Logic/Logic.h>

#include <AtomDestiny/Core/Utils.h>
#include <AtomDestiny/Core/MathUtils.h>
#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

UWeaponBase::UWeaponBase(const FObjectInitializer& objectInitializer):
    UADObject(objectInitializer)
{
}

void UWeaponBase::InitializeComponent()
{
    Super::InitializeComponent();
    
    LOG_CHECK_WARNING(m_weaponComponent != nullptr, TEXT("Weapon component is invalid, is it right?"));
        
    m_currentDamage = m_damage;
    m_currentAttackRange = m_attackRange;
    m_currentExplosionRadius = m_explosionRadius;
    m_currentCriticalChance = m_criticalChance;
    m_currentCriticalRate = m_criticalRate;
    m_currentReloadTime = m_reloadTime;

    m_minShotSqrtDistance = m_minShotDistance * m_minShotDistance;
    
    const std::vector params = {
        EObjectParameters::Reload,
        EObjectParameters::ExplosionRadius,
        EObjectParameters::Damage,
        EObjectParameters::CriticalRate,
        EObjectParameters::CriticalChance,
        EObjectParameters::Range
    };
    
    AddNewParameters(params);

    AtomDestiny::Utils::PreloadBlueprint(m_projectileBlueprint, BlueprintPreloadCount);
}

void UWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    
    m_weaponAnimation = GET_INTERFACE(WeaponAnimation);

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
        LOG_WARNING(TEXT("Target for weapon is invalid"));
        return;
    }

    const auto [angle, rotation] = AtomDestiny::RotationToTarget(m_weaponComponent.Get(), m_target.Get(), deltaTime, m_rotateSpeed);
    
    m_isRotatedOnTarget = (FMath::Abs(angle) < m_attackAngle);
    m_weaponComponent->SetWorldRotation(rotation);
}

void UWeaponBase::RotateToRoot(float deltaTime) const
{
    if (!m_target.IsValid() && m_rotatedWeapon)
    {
        const FQuat rotation = AtomDestiny::RotationToRoot(m_weaponComponent.Get(), GetOwner(), deltaTime, m_rotateSpeed);
        m_weaponComponent->SetWorldRotation(rotation);
    }
}

bool UWeaponBase::CheckRaycastToTarget(const FVector& origin, const FVector& direction, const TWeakObjectPtr<AActor>& target, FHitResult* hitResult) const
{
    // You can use this to customize various properties about the trace
    FCollisionQueryParams params;
    params.AddIgnoredActor(GetOwner());

    // The hit result gets populated by the line trace
    if (FHitResult hit; GetWorld()->LineTraceSingleByChannel(hit, origin, direction, m_layerMask, params))
    {
        if (hitResult != nullptr)
            *hitResult = hit;
        
        return hit.GetActor() == target;
    }
    
    return false;
}

FAsyncCoroutine UWeaponBase::FiringDelay()
{
    co_await Coroutines::Latent::Seconds(m_reloadTime);

    if (m_weaponAnimation != nullptr)
        m_weaponAnimation->SetDefaultState();
        
    m_firing = false;
}

void UWeaponBase::RecalculateParameter(EObjectParameters parameter)
{
    if (!GetParameterAvailable(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("U try to recalculate not available parameter"));
        return;
    }
    
    switch (parameter)
    {
    case EObjectParameters::ExplosionRadius:
        m_currentExplosionRadius = CalculateParametersFromAll(m_explosionRadius, parameter);
        break;

    case EObjectParameters::Reload:
        m_currentReloadTime = CalculateParametersFromAll(m_reloadTime, parameter);
        break;

    case EObjectParameters::Damage:
        m_currentDamage = CalculateParametersFromAll(m_damage, parameter);
        break;

    case EObjectParameters::CriticalChance:
        m_currentCriticalChance = CalculateParametersFromAll(m_criticalChance, parameter);
        break;

    case EObjectParameters::CriticalRate:
        m_currentCriticalRate = CalculateParametersFromAll(m_criticalRate, parameter);
        break;

    case EObjectParameters::Range:
        {
            if (const TScriptInterface<ILogic> logic = GET_INTERFACE(Logic); logic != nullptr)
            {
                m_currentAttackRange = CalculateParametersFromAll(m_attackRange, parameter);
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
            if (const TScriptInterface<ILogic> logic = GET_INTERFACE(Logic); logic != nullptr)
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
