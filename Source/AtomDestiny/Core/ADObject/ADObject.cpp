﻿#include "ADObject.h"

using namespace AtomDestiny;

// IParameterizable

void ADObject::AddParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement)
{
    if (enhancement.enhancementObject.IsValid() && enhancement.value >= 0)
        AddToParameter(parameter, enhancement);
}

void ADObject::RemoveParameter(ObjectParameters parameter, const GameObject& enhancementObject)
{
    if (enhancementObject.IsValid())
        RemoveFromParameter(parameter, enhancementObject);
}

void ADObject::ZeroParameter(ObjectParameters parameter, const ParameterZeroPack& pack)
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

float ADObject::InterpretParameterModifier(float baseValue, const ParameterEnhancement& enhancement)
{
    auto result = 0.0f;
    const auto value = std::abs(enhancement.value);

    switch (enhancement.modifier)
    {
    case ParameterEnhancement::Modifier::Plus:
        result = value;
        break;

    case ParameterEnhancement::Modifier::PlusCoefficient:
        result = baseValue * value;
        break;

    case ParameterEnhancement::Modifier::Minus:
        result = -value;
        break;

    case ParameterEnhancement::Modifier::MinusCoefficient:
        result = -(baseValue * value);
        break;

    default:
        break;
    }

    return result;
}

std::vector<ObjectParameters> ADObject::GetParameterTypes() const
{
    std::vector<ObjectParameters> objectParameters;
    objectParameters.reserve(m_enhancementParameters.size());

    for ([[maybe_unused]] const auto& [key, value] : m_enhancementParameters)
        objectParameters.push_back(key);
    
    return objectParameters;
}

std::vector<ParameterEnhancement>& ADObject::GetParameterEnhancementList(ObjectParameters parameter)
{
    const auto iter = m_enhancementParameters.find(parameter);

    if (iter == m_enhancementParameters.cend())
        throw std::logic_error("Trying to get non existing parameter");

    return iter->second.second;
}

bool ADObject::GetParameterAvailable(ObjectParameters parameter)
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

std::vector<GameObjectWeak>& ADObject::GetParameterZeroList(ObjectParameters parameter)
{
    const auto iter = m_enhancementParameters.find(parameter);

    if (iter == m_enhancementParameters.cend())
        throw std::logic_error("Trying to get non existing parameter");

    return iter->second.first;
}

void ADObject::AddNewParameter(ObjectParameters parameter)
{
    if (!m_enhancementParameters.count(parameter))
        m_enhancementParameters.emplace(parameter, GameObjectPairParameterList{});
}

void ADObject::AddNewParameters(const std::vector<ObjectParameters>& parameters)
{
    for (const auto parameter : parameters)
        AddNewParameter(parameter);
}

void ADObject::AddToParameter(ObjectParameters parameter, const ParameterEnhancement& enhancement)
{
    if (!m_enhancementParameters.count(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("You try to enhance non existing parameter"));
        return;
    }

    std::vector<ParameterEnhancement>& enhancementList = GetParameterEnhancementList(parameter);
    const auto predicate = [&enhancement](const ParameterEnhancement& element) {
        const GameObject ptr = enhancement.enhancementObject.Pin();
        return element.enhancementObject.HasSameObject(ptr.Get());
    };

    if (const auto iter = std::find_if(std::cbegin(enhancementList), std::cend(enhancementList), predicate); iter == std::cend(enhancementList))
        enhancementList.push_back(enhancement);
    
    if (GetParameterAvailable(parameter))
        Recalculate(parameter);
}

void ADObject::RemoveFromParameter(ObjectParameters parameter, const GameObject& enhanceObject)
{
    if (!m_enhancementParameters.count(parameter))
    {
        UE_LOG(LogTemp, Warning, TEXT("You try to remove enhancement object from non existing parameter"));
        return;
    }

    std::vector<ParameterEnhancement>& enhancementList = GetParameterEnhancementList(parameter);
    const auto predicate = [&enhanceObject](const ParameterEnhancement& element) {
        return element.enhancementObject.HasSameObject(enhanceObject.Get());
    };

    if (const auto iter = std::find_if(std::begin(enhancementList), std::end(enhancementList), predicate); iter != std::cend(enhancementList))
    {
        enhancementList.erase(iter);

        if (GetParameterAvailable(parameter))
            Recalculate(parameter);
    }
}

void ADObject::RemoveNullParameters(ObjectParameters parameter)
{
    if (const auto iter = m_enhancementParameters.find(parameter); iter != m_enhancementParameters.cend())
    {
        std::vector<ParameterEnhancement>& parameters = iter->second.second;
        const auto currentEnd = std::remove_if(std::begin(parameters), std::end(parameters), [](const ParameterEnhancement& element) {
            return !element.enhancementObject.IsValid();
        });

        parameters.erase(currentEnd, std::cend(parameters));
    }
}

void ADObject::Recalculate(ObjectParameters parameter)
{
    RemoveNullParameters(parameter);

    if (GetParameterAvailable(parameter))
        RecalculateParameter(parameter);
}
