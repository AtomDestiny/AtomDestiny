#include "ShieldObjectStateBase.h"

#include <AtomDestiny/Core/Logger.h>

double UShieldObjectStateBase::GetUnitShield() const
{
    return m_currentShieldValue;
}

EShieldType UShieldObjectStateBase::GetShieldType() const
{
    return m_shieldType;
}

double UShieldObjectStateBase::GetMaxUnitShield() const
{
    return m_currentMaxShield;
}

double UShieldObjectStateBase::GetShieldAbsorbation() const
{
    return m_currentAbsorbation;
}

double UShieldObjectStateBase::GetBaseMaxUnitShield() const
{
    return m_maxShieldValue;
}

double UShieldObjectStateBase::GetBaseShieldAbsorbation() const
{
    return m_shieldAbsorbation;
}

const FBalanceParameters& UShieldObjectStateBase::GetShieldAdditionalParameters() const
{
    return m_shieldBalanceParameters;
}

void UShieldObjectStateBase::InitializeComponent()
{
    Super::InitializeComponent();

    m_currentShieldValue = m_maxShieldValue;
    m_currentMaxShield = m_currentShieldValue;
    m_currentAbsorbation = m_shieldAbsorbation;

    AddNewParameter(EObjectParameters::MaxShield);
    AddNewParameter(EObjectParameters::Shield);
    AddNewParameter(EObjectParameters::Absorbation);
}

void UShieldObjectStateBase::BeginPlay()
{
    Super::BeginPlay();

    m_shieldBalanceParameters = AtomDestiny::Balance::CorrectBalanceParameters(m_shieldBalanceParameters);
}

void UShieldObjectStateBase::RecalculateParameter(EObjectParameters parameter)
{
    Super::RecalculateParameter(parameter);

    if (!GetParameterAvailable(parameter))
    {
        LOG_WARNING(TEXT("Recalculate not available parameter at UShieldObjectStateBase"));
        return;
    }
    
    switch (parameter)
    {
    case EObjectParameters::MaxShield:
        {
            m_currentShieldValue = CalculateParametersFromAll(m_maxShieldValue, parameter);

            if (m_currentShieldValue == m_maxShieldValue)
            {
                m_currentShieldValue = m_maxShieldValue;
            }
            else
            {
                m_currentShieldValue *= (m_maxShieldValue < m_currentMaxShield
                                             ? m_currentMaxShield / m_maxShieldValue
                                             : m_maxShieldValue / m_currentMaxShield);
            }
        }
        break;

    case EObjectParameters::Shield:
        {
            std::vector<FParameterEnhancement>& parameters = GetParameterEnhancementList(parameter);
            const double value = InterpretParameterModifier(m_currentShieldValue, parameters.empty() ? FParameterEnhancement{} : parameters.front());

            parameters.clear();

            m_currentShieldValue += value;

            if (m_currentShieldValue > m_currentMaxShield)
            {
                m_currentShieldValue = m_currentMaxShield;
            }
            else if (m_currentShieldValue < 0)
            {
                m_currentShieldValue = 0;
            }
        }
        break;

    case EObjectParameters::Absorbation:
        {
            m_currentAbsorbation = CalculateParametersFromAll(m_shieldAbsorbation, parameter);
        }
        break;

    default:
        break;
    }
}

void UShieldObjectStateBase::ZeroizeParameter(EObjectParameters parameter)
{
    Super::ZeroizeParameter(parameter);

    switch (parameter)
    {
    case EObjectParameters::MaxShield:
        m_currentMaxShield = 0;
        m_currentShieldValue = 0;
        break;

    case EObjectParameters::Shield:
        GetParameterZeroList(parameter).clear();
        m_currentShieldValue = 0;
        break;

    case EObjectParameters::Absorbation:
        m_currentAbsorbation = 0;
        break;

    default:
        break;
    }
}

void UShieldObjectStateBase::RegenerateShield()
{
    if (m_currentShieldValue + m_shieldRegenerateValue >= m_currentMaxShield)
        m_currentShieldValue = m_currentMaxShield;
    else
        m_currentShieldValue += m_shieldRegenerateValue;
}

FAsyncCoroutine UShieldObjectStateBase::StartShieldCoolDown()
{
    co_await Coroutines::Latent::Seconds(m_shieldRegenerateTime);
    m_regenerationAccess = true;
}

double UShieldObjectStateBase::GetDamageAfterShieldParameters(EWeaponType type, double damage) const
{
    double result = 0.0;

    switch (type)
    {
    case EWeaponType::Ballistic:
        result = damage * m_shieldBalanceParameters.ballisticResistance / 100.0;
        break;

    case EWeaponType::Energy:
        result = damage * m_shieldBalanceParameters.energyResistance / 100.0;
        break;

    case EWeaponType::Explosive:
        result = damage * m_shieldBalanceParameters.explosiveResistance / 100.0;
        break;

    case EWeaponType::Elemental:
        result = damage * m_shieldBalanceParameters.elementalResistance / 100.0;
        break;

    default:
        break;
    }
    
    return damage - result;
}
