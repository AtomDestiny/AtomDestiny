#include "UnitParameters.h"

void UUnitParameters::AddDamage(double damage, EWeaponType type, AActor* owner)
{
    double resultDamage = std::abs(damage) - m_currentDefence;
    resultDamage = std::max(resultDamage, AtomDestiny::Balance::MinDamageValue);
    resultDamage = GetDamageAfterDefenceType(resultDamage);
    resultDamage = GetDamageAfterDefenceParameters(type, resultDamage);

    // event
    if (m_action != nullptr)
        resultDamage = m_action->Invoke(resultDamage, EEventType::OnHealth);

    // change current health
    m_currentHealth -= resultDamage;
}

void UUnitParameters::TickComponent(float deltaTime, ELevelTick levelTick, FActorComponentTickFunction* func)
{
    Super::TickComponent(deltaTime, levelTick, func);
}
