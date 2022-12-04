#include "ADObject.h"

using namespace AtomDestiny;

std::vector<ObjectParameters> ADObject::GetParameterTypes() const
{
    std::vector<ObjectParameters> objectParameters;
    objectParameters.reserve(m_enhancementParameters.size());

    for (const auto& [key, value] : m_enhancementParameters)
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
        return element.enhancementObject.Get() == enhancement.enhancementObject.Get();
    };

    if (const auto iter = std::find_if(std::cbegin(enhancementList), std::cend(enhancementList), predicate); iter == std::cend(enhancementList))
        enhancementList.push_back(enhancement);
    
    if (GetParameterAvailable(parameter))
        Recalculate(parameter);
}
