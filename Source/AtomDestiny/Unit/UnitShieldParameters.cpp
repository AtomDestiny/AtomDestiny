#include "UnitShieldParameters.h"

void UUnitShieldParameters::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    if (m_regenerationAccess)
    {
        m_regenerationAccess = false;
        
        RegenerateShield();
        StartShieldCoolDown();
    }
    
    RenderShieldBar();
}

void UUnitShieldParameters::AddDamage(double damage, EWeaponType type, AActor* owner)
{
    Super::AddDamage(damage, type, owner);
}

void UUnitShieldParameters::RenderShieldBar()
{
    if (!m_shieldBar.IsValid() || m_hideBar)
    {
        return;
    }
    
    const double value = m_shieldBar->GetMaxValue() * m_currentShieldValue / m_currentMaxShield;
    
    m_shieldBar->SetValue(static_cast<float>(value));
    m_shieldBar->SetIsEnabled(m_currentShieldValue != m_currentMaxShield);
}
