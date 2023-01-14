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

void UScrapConstruction::Activate(bool reset)
{
    Super::Activate(reset);
    
    for (auto& [component, localTransform] : m_localTransforms)
    {
        component->SetRelativeTransform(localTransform);
    }
}
