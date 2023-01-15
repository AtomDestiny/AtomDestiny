#include "ScrapConstruction.h"

#include <AtomDestiny/Core/ActorComponentUtils.h>

void UScrapConstruction::BeginPlay()
{
    Super::BeginPlay();
    
    for (UStaticMeshComponent* component : AtomDestiny::Utils::GetComponents<UStaticMeshComponent>(GetOwner()))
    {
        TStrongObjectPtr componentPtr { component};
        m_localTransforms.Add(std::move(componentPtr), component->GetRelativeTransform());
    }
}

void UScrapConstruction::Deactivate()
{
    Super::Deactivate();

    const auto rootComponent = GetOwner()->GetRootComponent();
    
    for (auto& [component, localTransform] : m_localTransforms)
    {
        if (component.IsValid())
        {
            component->SetSimulatePhysics(false);
            component->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            component->SetRelativeTransform(localTransform);
        }
    }
}
