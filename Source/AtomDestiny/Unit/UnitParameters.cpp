#include "UnitParameters.h"

#include <AtomDestiny/ObjectState/Destroyable.h>

#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

void UUnitParameters::AddDamage(double damage, EWeaponType type, AActor* owner)
{
    double resultDamage = std::abs(damage) - m_currentDefence;
    resultDamage = std::max(resultDamage, AtomDestiny::Balance::MinDamageValue);
    resultDamage = GetDamageAfterDefenceType(resultDamage);
    resultDamage = GetDamageAfterDefenceParameters(type, resultDamage);
    
    // change current health
    m_currentHealth -= resultDamage;
}

void UUnitParameters::TickComponent(float deltaTime, ELevelTick levelTick, FActorComponentTickFunction* func)
{
    Super::TickComponent(deltaTime, levelTick, func);

    if (!m_healthBar.IsValid())
        return;
    
    const double value = m_healthBar->GetMaxValue() * m_currentHealth / m_currentMaxHealth;
    const bool showHeathBar = m_hideBar ? false : m_currentHealth != m_currentMaxHealth;
    
    m_healthBar->SetValue(static_cast<float>(value));
    m_healthBlueprint->SetIsEnabled(showHeathBar);

    // check current health on death
    if (m_currentHealth <= 0)
    {
        if (!m_isDead)
        {
            const TScriptInterface<IDestroyable> destroyable = GET_INTERFACE(Destroyable);

            if (destroyable == nullptr)
            {
                LOG_ERROR(TEXT("There is no IDestroyable component on Unit"));
                return;
            }

            destroyable->Destroy();
            m_isDead = !m_isDead;
        }
    }
}
