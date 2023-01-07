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
    // checks shield absorbation
    if (m_currentShieldValue > 0)
    {
        double shieldDmg = std::max(FMath::Abs(damage) - m_currentAbsorbation, AtomDestiny::Balance::MinDamageValue);
        shieldDmg = GetDamageAfterShieldParameters(type, shieldDmg);
        
        m_currentShieldValue -= shieldDmg;

        if (m_currentShieldValue < 0)
        {
            Super::AddDamage(damage, type, owner);
        }
    }
    else
    {
        Super::AddDamage(damage, type, owner);
    }
}

void UUnitShieldParameters::RenderShieldBar()
{
    if (m_healthBarWidget == nullptr || !m_healthBarWidget.IsValid() || m_hideBar)
        return;

    m_healthBarWidget->SetEnergyPercent(m_currentShieldValue / m_currentMaxShield);
    m_healthBarWidget->SetEnergyVisible(m_currentShieldValue != m_currentMaxShield);
}
