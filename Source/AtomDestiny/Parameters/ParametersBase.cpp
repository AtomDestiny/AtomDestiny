#include "ParametersBase.h"

#include <AtomDestiny/Core/Logger.h>

UParametersBase::UParametersBase(const FObjectInitializer& objectInitializer):
    UADObject(objectInitializer)
{
}

void UParametersBase::RegenerateHealth(double health)
{
    if (const auto value = FMath::Abs(health); m_currentHealth + value >= m_currentMaxHealth)
    {
        m_currentHealth = m_maxHealth;
    }
    else
    {
        m_currentHealth += value;
    }
}

bool UParametersBase::IsHealthMax() const
{
    return m_currentHealth == m_currentMaxHealth;
}

double UParametersBase::GetHealth() const
{
    return m_currentHealth;
}

EDefenceType UParametersBase::GetDefenceType() const
{
    return m_defenceType;
}

double UParametersBase::GetDefence() const
{
    return m_currentDefence;
}

double UParametersBase::GetMaxHealth() const
{
    return m_currentMaxHealth;
}

double UParametersBase::GetBaseMaxHealth() const
{
    return m_maxHealth;
}

double UParametersBase::GetBaseDefence() const
{
    return m_defence;
}

const FBalanceParameters& UParametersBase::GetDefenceAdditionalParameters() const
{
    return m_balanceParameters;
}

void UParametersBase::InitializeComponent()
{
    m_currentHealth = m_maxHealth;
    m_currentMaxHealth = m_currentHealth;
    m_currentDefence = m_defence;

    LOG_CHECK_WARNING(m_healthBlueprint != nullptr, TEXT("No health bar blueprint at Object state"));

    if (m_healthBlueprint.IsValid())
    {
        m_healthBar = CastChecked<USlider>(m_healthBlueprint);
    }
    
    AddNewParameter(EObjectParameters::Health);
    AddNewParameter(EObjectParameters::MaxHealth);
    AddNewParameter(EObjectParameters::Defence);
}

void UParametersBase::BeginPlay()
{
    Super::BeginPlay();
    
    m_balanceParameters = AtomDestiny::Balance::CorrectBalanceParameters(m_balanceParameters);
}

double UParametersBase::GetDamageAfterDefenceParameters(EWeaponType type, double damage) const
{
    double calculateResistanceValue = 0.0f;

    switch (type)
    {
    case EWeaponType::Ballistic:
        calculateResistanceValue = damage * m_balanceParameters.ballisticResistance / 100.0;
        break;

    case EWeaponType::Energy:
        calculateResistanceValue = damage * m_balanceParameters.energyResistance / 100.0;
        break;

    case EWeaponType::Explosive:
        calculateResistanceValue = damage * m_balanceParameters.explosiveResistance / 100.0;
        break;

    case EWeaponType::Elemental:
        calculateResistanceValue = damage * m_balanceParameters.elementalResistance / 100.0;
        break;

    default:
        break;
    }
    
    return damage - calculateResistanceValue;
}

double UParametersBase::GetDamageAfterDefenceType(float damage) const
{
    switch (m_defenceType)
    {
    case EDefenceType::Light:
        damage *= AtomDestiny::Balance::VsLightDefenceCoefficient;
        break;

    case EDefenceType::Medium:
        damage *= AtomDestiny::Balance::VsMediumDefenceCoefficient;
        break;

    case EDefenceType::Heavy:
        damage *= AtomDestiny::Balance::VsHeavyDefenceCoefficient;
        break;

    default:
        break;
    }

    return damage;
}

double UParametersBase::GetResultDamage(EWeaponType type, double damage) const
{
    double resultDamage = std::max(std::abs(damage) - m_currentDefence, AtomDestiny::Balance::MinDamageValue);
    resultDamage = GetDamageAfterDefenceType(resultDamage);
    resultDamage = GetDamageAfterDefenceParameters(type, resultDamage);

    return resultDamage;
}

void UParametersBase::RecalculateParameter(EObjectParameters parameter)
{
    if (!GetParameterAvailable(parameter))
    {
        LOG_WARNING(TEXT("Recalculate not available parameter at UObjectStateBase"));
        return;
    }
    
    switch (parameter)
    {
    case EObjectParameters::MaxHealth:
        {
            m_currentMaxHealth = CalculateParametersFromAll(m_maxHealth, parameter);
            
            if (m_currentHealth == m_maxHealth)
            {
                m_currentHealth = m_currentMaxHealth;
            }
            else
            {
                m_currentHealth *= (m_maxHealth < m_currentMaxHealth ? m_currentMaxHealth / m_maxHealth : m_maxHealth / m_currentMaxHealth);
            }
        }

        break;
    
    case EObjectParameters::Health:
        {
            std::vector<FParameterEnhancement>& parameters = GetParameterEnhancementList(parameter);
            const double value = InterpretParameterModifier(m_currentHealth, parameters.empty() ? FParameterEnhancement{} : parameters.front());
            
            parameters.clear();
            m_currentHealth += value;

            if (m_currentHealth > m_currentMaxHealth)
            {
                m_currentHealth = m_currentMaxHealth;
            }
            else if (m_currentHealth < 0)
            {
                m_currentHealth = 0;
            }

            break;
        }
    
    case EObjectParameters::Defence:
        m_currentDefence = CalculateParametersFromAll(m_defence, parameter);
        break;

    default:
        break;
    }
}

void UParametersBase::ZeroizeParameter(EObjectParameters parameter)
{
    switch (parameter)
    {
    case EObjectParameters::MaxHealth:
        m_currentMaxHealth = 0;
        m_currentHealth = 0;
        break;

    case EObjectParameters::Health:
        GetParameterZeroList(parameter).clear();
        m_currentHealth = 0;
        break;

    case EObjectParameters::Defence:
        m_currentDefence = 0;
        break;

    default:
        break;
    }
}
