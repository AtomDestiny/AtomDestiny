#include "ADObject.h"

UADObject::UADObject()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// IParameterizable

void UADObject::AddParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement)
{
    if (enhancement.enhancementObject.IsValid() && enhancement.value >= 0)
        AddToParameter(parameter, enhancement);
}

void UADObject::RemoveParameter(EObjectParameters parameter, AActor* enhancementObject)
{
    if (const TWeakObjectPtr<AActor> ptr { enhancementObject }; ptr.IsValid())
        RemoveFromParameter(parameter, ptr);
}

void UADObject::ZeroParameter(EObjectParameters parameter, const FParameterZeroPack& pack)
{
    if (!pack.zeroObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Pack zero object is null"));
        return;
    }

    if (!m_enhancementParameters.count(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("You try to zero non existing parameter"));
        return;
    }
    
    std::vector<GameObjectWeak>& zeroList = m_enhancementParameters.find(parameter)->second.first;

    if (const auto iter = std::find(std::begin(zeroList), std::end(zeroList), pack.zeroObject); iter != std::cend(zeroList))
    {
        zeroList.erase(iter);
    }
    else if (pack.status)
    {
        zeroList.push_back(pack.zeroObject);
        ZeroizeParameter(parameter);
    }
    
    Recalculate(parameter);
}

double UADObject::InterpretParameterModifier(double baseValue, const FParameterEnhancement& enhancement)
{
    auto result = 0.0f;
    const auto value = std::abs(enhancement.value);

    switch (enhancement.modifier)
    {
    case EParameterModifier::Plus:
        result = value;
        break;

    case EParameterModifier::PlusCoefficient:
        result = baseValue * value;
        break;

    case EParameterModifier::Minus:
        result = -value;
        break;

    case EParameterModifier::MinusCoefficient:
        result = -(baseValue * value);
        break;

    default:
        break;
    }

    return result;
}

std::vector<EObjectParameters> UADObject::GetParameterTypes() const
{
    std::vector<EObjectParameters> objectParameters;
    objectParameters.reserve(m_enhancementParameters.size());

    for ([[maybe_unused]] const auto& [key, value] : m_enhancementParameters)
        objectParameters.push_back(key);
    
    return objectParameters;
}

std::vector<FParameterEnhancement>& UADObject::GetParameterEnhancementList(EObjectParameters parameter)
{
    const auto iter = m_enhancementParameters.find(parameter);

    if (iter == m_enhancementParameters.cend())
        throw std::logic_error("Trying to get non existing parameter");

    return iter->second.second;
}

bool UADObject::GetParameterAvailable(EObjectParameters parameter)
{
    if (const auto iter = m_enhancementParameters.find(parameter); iter != m_enhancementParameters.end())
    {
        std::vector<GameObjectWeak>& gameObjects = iter->second.first;
        const auto currentEnd = std::remove_if(std::begin(gameObjects), std::end(gameObjects), [](const GameObjectWeak& gameObject) {
            return !gameObject.IsValid();
        });

        gameObjects.erase(currentEnd, std::cend(gameObjects));
        return gameObjects.empty();
    }

    return false;
}

std::vector<TWeakObjectPtr<AActor>>& UADObject::GetParameterZeroList(EObjectParameters parameter)
{
    const auto iter = m_enhancementParameters.find(parameter);

    if (iter == m_enhancementParameters.cend())
        throw std::logic_error("Trying to get non existing parameter");

    return iter->second.first;
}

void UADObject::AddNewParameter(EObjectParameters parameter)
{
    if (!m_enhancementParameters.count(parameter))
        m_enhancementParameters.emplace(parameter, GameObjectPairParameterList{});
}

void UADObject::AddNewParameters(const std::vector<EObjectParameters>& parameters)
{
    for (const auto parameter : parameters)
        AddNewParameter(parameter);
}

void UADObject::AddToParameter(EObjectParameters parameter, const FParameterEnhancement& enhancement)
{
    if (!m_enhancementParameters.count(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("You try to enhance non existing parameter"));
        return;
    }

    std::vector<FParameterEnhancement>& enhancementList = GetParameterEnhancementList(parameter);
    const auto predicate = [&enhancement](const FParameterEnhancement& element) {
        return element.enhancementObject.Get() == enhancement.enhancementObject.Get();
    };

    if (const auto iter = std::find_if(std::cbegin(enhancementList), std::cend(enhancementList), predicate); iter == std::cend(enhancementList))
        enhancementList.push_back(enhancement);
    
    if (GetParameterAvailable(parameter))
        Recalculate(parameter);
}

void UADObject::RemoveFromParameter(EObjectParameters parameter, const TWeakObjectPtr<AActor>& enhanceObject)
{
    if (!m_enhancementParameters.count(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("You try to remove enhancement object from non existing parameter"));
        return;
    }

    std::vector<FParameterEnhancement>& enhancementList = GetParameterEnhancementList(parameter);
    const auto predicate = [&enhanceObject](const FParameterEnhancement& element) {
        return element.enhancementObject.Get() == enhanceObject.Get();
    };

    if (const auto iter = std::find_if(std::begin(enhancementList), std::end(enhancementList), predicate); iter != std::cend(enhancementList))
    {
        enhancementList.erase(iter);

        if (GetParameterAvailable(parameter))
            Recalculate(parameter);
    }
}

double UADObject::CalculateParametersFromAll(const double startValue, EObjectParameters parameter) const
{
    const std::vector<FParameterEnhancement>& parameters = const_cast<UADObject*>(this)->GetParameterEnhancementList(parameter);
    const auto calculator = [&parameters, startValue]() {
        auto currentValue = startValue;

        for (const FParameterEnhancement p : parameters)
            currentValue += InterpretParameterModifier(startValue, p);

        return currentValue;
    };

    return calculator();
}

void UADObject::RemoveNullParameters(EObjectParameters parameter)
{
    if (const auto iter = m_enhancementParameters.find(parameter); iter != m_enhancementParameters.cend())
    {
        std::vector<FParameterEnhancement>& parameters = iter->second.second;
        const auto currentEnd = std::remove_if(std::begin(parameters), std::end(parameters), [](const FParameterEnhancement& element) {
            return !element.enhancementObject.IsValid();
        });

        parameters.erase(currentEnd, std::cend(parameters));
    }
}

void UADObject::Recalculate(EObjectParameters parameter)
{
    RemoveNullParameters(parameter);

    if (GetParameterAvailable(parameter))
        RecalculateParameter(parameter);
}
