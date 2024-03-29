﻿#include "UnitState.h"

#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>
#include <AtomDestiny/Core/ADObject/Parameterizable.h>

#include <AtomDestiny/Navigation/Navigator.h>

namespace
{
    // Returns current navigation from Owner actor
    TWeakObjectPtr<ANavigator> GetNavigation(AActor* owner)
    {
        const APawn* pawn = CastChecked<APawn>(owner);
        return MakeWeakObjectPtr(CastChecked<ANavigator>(pawn->Controller));
    }
    
} // namespace

UUnitState::UUnitState(const FObjectInitializer& objectInitializer):
    UActorComponent(objectInitializer)
{
}

double UUnitState::GetVelocity() const
{
    // TODO: when we should have an animator, change speed of animator
    // if (unitAnimation != null)
    //     speed = GetComponent<Animator>().speed;
    // else
    return m_logic->GetVelocity();
}

const TArray<TScriptInterface<IWeapon>>& UUnitState::GetWeapons() const
{
    return m_logic->GetAllWeapon();
}

bool UUnitState::IsWeaponFiring() const
{
    const TArray<TScriptInterface<IWeapon>>& weapon = m_logic->GetAllWeapon();
    return std::any_of(std::begin(weapon), std::end(weapon), [](const TScriptInterface<IWeapon>& w) {
        return w->IsFiring();
    });
}

double UUnitState::GetMaxDamage() const
{
    double maxValue = 0;
    
    for (const TScriptInterface<IWeapon>& w : m_logic->GetAllWeapon())
    {
        if (const double damage = w->GetDamage(); maxValue < damage)
            maxValue = damage;
    }

    return maxValue;
}

double UUnitState::GetMinDamage() const
{
    const TArray<TScriptInterface<IWeapon>>& weapons = GetWeapons();

    if (weapons.IsEmpty())
        return 0;

    double minValue = weapons[0]->GetDamage();
    
    for (int32 i = 1; i < weapons.Num(); ++i)
    {
        if (const double damage = weapons[i]->GetDamage(); minValue > damage)
            minValue = damage;
    }

    return minValue;
}

double UUnitState::GetMaxAttackRange() const
{
    double maxValue = 0;
    
    for (const TScriptInterface<IWeapon>& w : m_logic->GetAllWeapon())
    {
        if (const double damage = w->GetAttackRange(); maxValue < damage)
            maxValue = damage;
    }

    return maxValue;
}

double UUnitState::GetMinAttackRange() const
{
    const TArray<TScriptInterface<IWeapon>>& weapons = GetWeapons();

    if (weapons.IsEmpty())
        return 0;

    double minValue = weapons[0]->GetAttackRange();
    
    for (int32 i = 1; i < weapons.Num(); ++i)
    {
        if (const double damage = weapons[i]->GetAttackRange(); minValue > damage)
            minValue = damage;
    }

    return minValue;
}

bool UUnitState::HasAnyWeaponCritical() const
{
    const TArray<TScriptInterface<IWeapon>>& weapon = m_logic->GetAllWeapon();
    return std::any_of(std::begin(weapon), std::end(weapon), [](const TScriptInterface<IWeapon>& w) {
        return w->IsCritical();
    });
}

bool UUnitState::HasAnyWeaponAoe() const
{
    const TArray<TScriptInterface<IWeapon>>& weapon = m_logic->GetAllWeapon();
    return std::any_of(std::begin(weapon), std::end(weapon), [](const TScriptInterface<IWeapon>& w) {
        return w->IsAoe();
    });
}

FUnitStateParameters UUnitState::GetUnitState() const
{
    return FUnitStateParameters {
        .velocity = GetVelocity(),
        .weaponCount = GetWeaponCount(),
        .currentHealth = GetCurrentHealth(),
        .maxHealth = GetMaxHealth(),
        .currentShieldValue = GetCurrentShieldValue(),
        .maxShieldValue = GetMaxShieldValue(),
        .defence = GetDefence(),
        .shieldType = GetShieldType(),
        .defenceType = GetDefenceType(),
        .unitType = GetUnitType(),
        .hasShield = HasShield(),
        .weaponList = GetWeapons(),
        .maxDamage = GetMaxDamage(),
        .minDamage = GetMinDamage(),
        .maxRange = GetMaxAttackRange(),
        .minRange = GetMinAttackRange(),
        .isAnyWeaponCritical = HasAnyWeaponCritical(),
        .isAnyWeaponAoe = HasAnyWeaponAoe(),
        .minerals = GetCost(),
    };
}

void UUnitState::AddDamage(double damage, EWeaponType weaponType, AActor* damageOwner)
{
    m_objectState->AddDamage(damage, weaponType, damageOwner);
}

void UUnitState::RegenerateCurrentHealth(double health)
{
    m_objectState->RegenerateHealth(health);
}

void UUnitState::AddParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement)
{
    switch (parameter)
    {
    case EObjectParameters::Damage:
    case EObjectParameters::CriticalRate:
    case EObjectParameters::CriticalChance:
    case EObjectParameters::ExplosionRadius:
    case EObjectParameters::Range:
    case EObjectParameters::Reload:
        {
            for (const TScriptInterface<IWeapon>& w : GetWeapons())
            {
                Cast<IParameterizable>(w.GetInterface())->AddParameter(parameter, enhancement);
            }
            
            break;
        }
        
    case EObjectParameters::Velocity:
        Cast<IParameterizable>(m_logic.GetInterface())->AddParameter(parameter, enhancement);
        break;

    case EObjectParameters::MaxHealth:
    case EObjectParameters::Health:
    case EObjectParameters::Defence:
    case EObjectParameters::MaxShield:
    case EObjectParameters::Shield:
    case EObjectParameters::Absorption:
        {
            if (m_shield != nullptr)
                Cast<IParameterizable>(m_shield.GetInterface())->AddParameter(parameter, enhancement);
        }
        break;

    default:
        break;
    }
}

void UUnitState::RemoveParameter(EObjectParameters parameter, AActor* enhanceObject)
{
    switch (parameter)
    {
    case EObjectParameters::Damage:
    case EObjectParameters::CriticalRate:
    case EObjectParameters::CriticalChance:
    case EObjectParameters::ExplosionRadius:
    case EObjectParameters::Range:
    case EObjectParameters::Reload:
        {
            for (const TScriptInterface<IWeapon>& w : GetWeapons())
            {
                Cast<IParameterizable>(w.GetInterface())->RemoveParameter(parameter, enhanceObject);
            }

            break;
        }

    case EObjectParameters::Velocity:
        Cast<IParameterizable>(m_logic.GetInterface())->RemoveParameter(parameter, enhanceObject);
        break;

    case EObjectParameters::MaxHealth:
    case EObjectParameters::Health:
    case EObjectParameters::Defence:
    case EObjectParameters::MaxShield:
    case EObjectParameters::Shield:
    case EObjectParameters::Absorption:
        {
            if (m_shield != nullptr)
                Cast<IParameterizable>(m_shield.GetInterface())->RemoveParameter(parameter, enhanceObject);
        }
        break;

    default:
        break;
    }
}

void UUnitState::ZeroParameter(EObjectParameters parameter, const FParameterZeroPack& pack)
{
    switch (parameter)
    {
    case EObjectParameters::Damage:
    case EObjectParameters::CriticalRate:
    case EObjectParameters::CriticalChance:
    case EObjectParameters::ExplosionRadius:
    case EObjectParameters::Range:
    case EObjectParameters::Reload:
        {
            for (const TScriptInterface<IWeapon>& w : GetWeapons())
            {
                Cast<IParameterizable>(w.GetInterface())->ZeroParameter(parameter, pack);
            }

            break;
        }

    case EObjectParameters::Velocity:
        Cast<IParameterizable>(m_logic.GetInterface())->ZeroParameter(parameter, pack);
        break;

    case EObjectParameters::MaxHealth:
    case EObjectParameters::Health:
    case EObjectParameters::Defence:
    case EObjectParameters::MaxShield:
    case EObjectParameters::Shield:
    case EObjectParameters::Absorption:
        {
            if (m_shield != nullptr)
                Cast<IParameterizable>(m_shield.GetInterface())->ZeroParameter(parameter, pack);
        }
        break;


    default:
        break;
    }
}

void UUnitState::BeginPlay()
{
    Super::BeginPlay();

    m_logic = GET_INTERFACE(Logic);
    m_objectState = GET_INTERFACE(Parameters);
    m_shield = GET_INTERFACE(Shield);
    m_animation = GET_INTERFACE(Animation);
    
    ENSURE(m_groundPoint != nullptr, TEXT("Ground point is invalid"));
    ENSURE(m_objectState != nullptr, TEXT("Unit object state is invalid"));
    ENSURE(m_logic != nullptr, TEXT("Unit logic is invalid"));
}

void UUnitState::SetEnabled(bool enabled)
{
    CastChecked<UActorComponent>(m_logic.GetInterface())->SetComponentTickEnabled(enabled);
    GetNavigation(GetOwner())->SetActorTickEnabled(enabled);
    
    for (const TScriptInterface<IWeapon>& weapon : GetWeapons())
    {
        CastChecked<UActorComponent>(weapon.GetInterface())->SetComponentTickEnabled(enabled);
    }

    // TODO: make animation enabled
    //     if (unitAnimation != null)
    //         GetComponent<Animator>().enabled = enabled;
}
