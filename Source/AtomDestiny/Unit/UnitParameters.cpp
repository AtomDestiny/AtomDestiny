#include "UnitParameters.h"

#include <AtomDestiny/Behaviour/Destroyable.h>

#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

UUnitParameters::UUnitParameters(const FObjectInitializer& objectInitializer):
    UParametersBase(objectInitializer)
{
    SetTickEnabled(true);
}

void UUnitParameters::AddDamage(double damage, EWeaponType type, AActor* owner)
{
    if (m_currentHealth <= 0)
    {
        LOG_ERROR(TEXT("Current unit heath equal or lower that zero. Something went wrong"));
        return;
    }

    // calculate result damage change current health
    const double resultDamage = GetResultDamage(type, damage);
    m_currentHealth -= resultDamage;

    CheckHealthState();
}

void UUnitParameters::TickComponent(float deltaTime, ELevelTick levelTick, FActorComponentTickFunction* func)
{
    Super::TickComponent(deltaTime, levelTick, func);
    
    RenderHeathBar();
}

void UUnitParameters::RenderHeathBar()
{
    if (!m_healthBar.IsValid() || m_hideBar)
    {
        return;
    }
    
    const double value = m_healthBar->GetMaxValue() * m_currentHealth / m_currentMaxHealth;
    
    m_healthBar->SetValue(static_cast<float>(value));
    m_healthBar->SetIsEnabled(m_currentHealth != m_currentMaxHealth);
}

void UUnitParameters::CheckHealthState()
{
    // check current health on death
    if (m_currentHealth <= 0 && !m_isDead)
    {
        const TScriptInterface<IDestroyable> destroyable = GET_INTERFACE(Destroyable);

        if (destroyable == nullptr)
        {
            LOG_ERROR(TEXT("There is no IDestroyable component on Unit, so Unit params can not imitate a Destroy"));
            return;
        }

        if (!destroyable->IsDestroyed())
        {
            destroyable->Destroy();
        }

        m_isDead = true;
    }
}
