#include "ScrapConstruction.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>

void UScrapConstruction::BeginPlay()
{
    Super::BeginPlay();
    
    for (UStaticMeshComponent* component : AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(GetOwner()))
    {
        m_localTransforms.Add(MakeWeakObjectPtr(component), component->GetRelativeTransform());
    }
}

void UScrapConstruction::SetActive(bool newActive, bool reset)
{
    Super::SetActive(newActive, reset);

    if (!newActive)
    {
        return;
    }
    
    for (auto& [component, localTransform] : m_localTransforms)
    {
        component->SetRelativeTransform(localTransform);
    }
}
